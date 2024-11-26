obj-m := irm.o
rust_src := target/debug/libirm.a

irm-objs := $(rust_src)

all:
    cargo build --target x86_64-unknown-none
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
    cargo clean
