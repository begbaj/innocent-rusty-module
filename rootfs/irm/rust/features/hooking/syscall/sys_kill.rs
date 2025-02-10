use kernel::c_str;
use kernel::{bindings::pt_regs, pr_info};

use super::{hook, KProbe};

pub(crate) fn sys_hook() {
    let symbol = c_str!("__x64_sys_kill");

    hook(symbol.as_char_ptr(), pre_handler, post_handler).expect("Failed to hook sys_kill");
}

pub(crate) unsafe extern "C" fn pre_handler(_p: *mut KProbe, _regs: *mut pt_regs) -> i32 {
    pr_info!("kill invoked");
    1
}

pub(crate) unsafe extern "C" fn post_handler(
    _p: *mut KProbe,
    _regs: *mut pt_regs,
    _flags: u64,
) -> i32 {
    1
}
