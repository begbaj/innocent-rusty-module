qemu-system-x86_64 \
	-M pc \
	-m 512M \
	-serial mon:stdio \
	-kernel linux/arch/x86_64/boot/bzImage \
	-initrd rootfs.cpio.gz \
	-accel kvm \
	-append "root=/dev/mem" \

