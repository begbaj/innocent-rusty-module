#![no_std]
#![feature(allocator_api, global_asm)]

use kernel::prelude::*;

module! {
    type: MyKernelModule,
    name: "my_kernel_module",
    author: "Your Name",
    description: "A simple kernel module written in Rust",
    license: "GPL",
}

struct MyKernelModule;

impl KernelModule for MyKernelModule {
    fn init() -> Result<Self> {
        pr_info!("Hello, kernel from Rust!\n");
        Ok(MyKernelModule)
    }
}

impl Drop for MyKernelModule {
    fn drop(&mut self) {
        pr_info!("Goodbye, kernel from Rust!\n");
    }
}
