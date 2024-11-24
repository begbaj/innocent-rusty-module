# innocent-rusty-module
A (not so) innocent rusty module for Linux.

# Notes
- Somen useful documentation can be found under `linux/Documentation/rust/`.


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


configure via menuconfig

```
cd linux
make LLVM=../llvm defconfig
make LLVM=../llvm menuconfig
```

### Rustup
```bash
cd linux
rustup override set stable
rustup component add rust-src
rustup component add rustfmt
rustup component add clippy
cargo install --locked bindgen-cli
```



