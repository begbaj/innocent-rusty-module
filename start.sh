source env

cd linux-$LINUX_VERSION/
make LLVM=../llvm -j$(nproc)
cd ..

cd rootfs/irm/
bash make.sh
cd ../../

bash packrootfs.sh

qemu-system-x86_64 \
	-M pc \
	-m 512M \
	-serial mon:stdio \
	-kernel linux-$LINUX_VERSION/arch/x86_64/boot/bzImage \
	-initrd rootfs.cpio.gz \
	-accel kvm \
	-append "root=/dev/mem" \

