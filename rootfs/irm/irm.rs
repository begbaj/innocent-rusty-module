//! Innocent Rusty module
#![allow(unused)]

use core::format_args;
use kernel::bindings::{ftrace_ops, pt_regs, __symbol_get_gpl, __symbol_get};
use kernel::error::Result;
use kernel::prelude::*;

module! {
    type: IRM,
    name: "irm",
    author: "Began Bajrami",
    description: "just a chill module",
    license: "GPL",
}

macro_rules! hook {
    ($name:expr, $hook:expr, $orig:expr) => {
        Hook {
            name: concat!("__x64_", $name),
            function: $hook,
            original: $orig,
            address: None,
            ops: None,
        }
    };
}

struct Hook {
    name: &'static core::ffi::c_char,
    function: fn(),
    original: fn(),

    address: Option<u64>,
    ops: Option<ftrace_ops>,
}

struct IRM;

// eseguire un println ogni qual volta viene chiamato un mkdir

type OrigT = unsafe extern "C" fn(regs: *const pt_regs) -> core::ffi::c_long;
static mut ORIG_MKDIR: Option<OrigT> = None;

impl IRM {
    fn ftrace_thunk() {
        todo!();
    }
    fn resolve_hook_address() {
        todo!();
    }

    fn get_addr_ptr(sysc: &[u8]) -> Option<OrigT> {
        let mut symb = CStr::from_bytes_with_nul(sysc).unwrap();
        let mut orig: Option<OrigT> = None;
        unsafe {
            orig = Some(core::mem::transmute(symb.as_char_ptr()));
        }
        orig
    }

    fn install_hook() {
        //let mut symbols = KVec::new();
        // symbols.push(CStr::from_bytes_with_nul(b"sys_mkdir\0").unwrap(), GFP_KERNEL);
        // symbols.push(CStr::from_bytes_with_nul(b"sys_read\0").unwrap(), GFP_KERNEL);
        // symbols.push(CStr::from_bytes_with_nul(b"sys_open\0").unwrap(), GFP_KERNEL);
        // symbols.push(CStr::from_bytes_with_nul(b"__x64_sys_mkdir\0").unwrap(), GFP_KERNEL);
        // symbols.push(CStr::from_bytes_with_nul(b"__x64_sys_read\0").unwrap(), GFP_KERNEL);
        // let mut symb = CStr::from_bytes_with_nul(b"__x64_sys_mkdir\0").unwrap();
        // let mut orig: Option<OrigT> = None;
        // unsafe {
        //     orig = Some(core::mem::transmute(symb.as_char_ptr()));
        // }

        let orig_mkdir = IRM::get_addr_ptr(b"__x64_sys_mkdir\0");
        let orig_read  = IRM::get_addr_ptr(b"__x64_sys_read\0");
        let orig_open  = IRM::get_addr_ptr(b"__x64_sys_read\0");

        pr_info!("address {:p}", orig_mkdir.unwrap() as *const core::ffi::c_void);
        pr_info!("address {:p}", orig_read.unwrap() as *const core::ffi::c_void);
        pr_info!("address {:p}", orig_open.unwrap() as *const core::ffi::c_void);

    }
    fn uninstall_hook() {
        todo!();
    }
}

impl kernel::Module for IRM {
    fn init(_module: &'static ThisModule) -> Result<Self> {
        pr_info!("IRM activated\n");
        IRM::install_hook();
        Ok(IRM)
    }
}

impl Drop for IRM {
    fn drop(&mut self) {
        pr_info!("Why? Nobody likes me, I'm an Innocent chill module ;( \n");
    }
}
