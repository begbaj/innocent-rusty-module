
use std::process::Command;

fn main() {
    let kernel_dir = "/lib/modules/$(uname -r)/build"; // Path to kernel headers
    let out_dir = std::env::var("OUT_DIR").unwrap();

    // Run the kernel's build system with Rust support
    let status = Command::new("make")
        .arg("-C")
        .arg(kernel_dir)
        .arg("M=$(pwd)")
        .arg("rust")
        .arg(format!("O={out_dir}"))
        .status()
        .expect("Failed to build kernel module");

    if !status.success() {
        panic!("Kernel module build failed!");
    }
}
