//! Innocent Rusty module
#![allow(unused)]

use kernel::prelude::*;

module! {
    type: Cr0Monitor,
    name: "cr0_monitor",
    author: "Your Name",
    description: "Monitors changes to the CR0 register",
    license: "GPL",
}

struct Cr0Monitor;

impl kernel::Module for Cr0Monitor {
    fn init(_module: &'static ThisModule) -> Result<Self> {
        pr_info!("CR0 Monitor: Initializing...\n");
        let initial_cr0 = unsafe { cr0_read() };
        pr_info!("Initial CR0 Value: {:#x}", initial_cr0);

        // Add periodic monitoring logic if needed
        Ok(Cr0Monitor)
    }
}

unsafe fn cr0_read() -> u64 {
    let mut cr0: u64;
    unsafe {
        core::arch::asm!(
            "mov {0}, cr0",
            out(reg) cr0,
            options(nostack, preserves_flags)
        );
    }
    cr0
}
