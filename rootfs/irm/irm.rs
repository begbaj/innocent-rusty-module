//! Innocent Rusty module
#![allow(unused)]

use core::format_args;
use kernel::bindings::{ftrace_ops, pt_regs};
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
    name: &'static str,
    function: fn(),
    original: fn(),

    address: Option<u64>,
    ops: Option<ftrace_ops>,
}

struct IRM;

// eseguire un println ogni qual volta viene chiamato un mkdir

impl IRM {
    fn ftrace_thunk() {
        todo!();
    }
    fn resolve_hook_address() {
        todo!();
    }
    fn install_hook(hook: &mut Hook) {
/** sadly this doesn't work since kallsyms_lookup_name is not anymore exported on
 * more recent versions of linux (5.7 and above)
 *      unsafe {
 *          hook.address = Some(kallsyms_lookup_name(
 *              CStr::from_bytes_with_nul(hook.name.as_bytes())
 *                  .unwrap()
 *                  .as_char_ptr(),
 *          ));
 *      }
 *      pr_debug!("address for {}: {}", hook.address.unwrap(), hook.name);
**/
        todo!()
    }
    fn uninstall_hook() {
        todo!();
    }
}

impl kernel::Module for IRM {
    fn init(_module: &'static ThisModule) -> Result<Self> {
        pr_info!("IRM activated\n");
        // let mut hk = hook!("sys_read", || {}, || {});
        // IRM::install_hook(&mut hk);
        Ok(IRM)
    }
}

impl Drop for IRM {
    fn drop(&mut self) {
        pr_info!("Why? Nobody likes me, I'm an Innocent chill module ;( \n");
    }
}
