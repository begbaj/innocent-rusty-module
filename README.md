# innocent-rusty-module
![Chill Module just wants to sit on your kernel](chill-rust-module.jpg "Just a chill rust module")
A (not so) innocent rusty module for Linux.

# Resources
[Step by step personal notes](https://app.affine.pro/workspace/19ab237e-8c00-46b3-9c4f-0d4b0205db36/feMuxeLUV9CSbIaPjOMYd?mode=page)

[Understanding how a rootkit works](https://app.affine.pro/workspace/19ab237e-8c00-46b3-9c4f-0d4b0205db36/pn-GbwR9OpXvxIlEuBYwD?mode=page)

[State of Art](https://app.affine.pro/workspace/19ab237e-8c00-46b3-9c4f-0d4b0205db36/BHp8DoN8sAjhweM8_BGye?mode=page)

# Notes
- Some useful documentation can be found under `linux/Documentation/rust/`.
- Rust documentaion is awfully lacking usefull information.


# environment setup on ArchLinux
Download and extract
- linux kernel 6.12 [link](https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.12.tar.xz)
- busybox 1.37.0 [link](https://busybox.net/downloads/busybox-1.37.0.tar.bz2)
- LLVM toolchain with Rust x86_64 [link](https://mirrors.edge.kernel.org/pub/tools/llvm/files/llvm-19.1.4-x86_64.tar.xz)

```bash
tar -xf linux.tar.xz
tar -xf busybox.tar.bz2
tar -xf llvm.tar.bz2
```
## requirements on the host machine

### Arch repository
```bash
sudo pacman -S qemu qemu-system-aarch64 bc base-devel
```

## configure kernel
check if all is correct for Rust compiling

```
$ cd linux
$ make LLVM=1 rustavailable
> Rust is available!
```

minimum configuration
```bash
make allnoconfig qemu-busybox-min.config rust.config
```
but the previous doesn't really work (for some reason I do not have the qemu-busybox-min.config on my linux source code sadly)

so use this instead:
```bash
make LLVM=../llvm defconfig rust.config
make LLVM=../llvm menuconfig
```
in menuconfig enable the rust samples.

### Rustup
```bash
cd linux
rustup override set nighlty
rustup component add rust-src
rustup component add rustfmt
rustup component add clippy
cargo install --locked bindgen-cli
```

