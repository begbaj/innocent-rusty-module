#!/bin/bash
source env
clear
REPACK=true
REMAKE=true
while true; do
	if $REMAKE; then
		cd linux-$LINUX_VERSION/
			make LLVM=../llvm -j$(nproc)
		cd ..
	fi

	if $REPACK; then
		bash packrootfs.sh
	fi

	echo "run gdb and type 'target remote :1234'"
	echo "to restart qemu, type [C-a,x]"

	# -s -S \ add for gdb support
	qemu-system-x86_64 \
		-M pc \
		-m 512M \
		-serial mon:stdio \
		-kernel linux-$LINUX_VERSION/arch/x86_64/boot/bzImage \
		-initrd rootfs.cpio.gz \
		-accel kvm \
		-append "console=ttyS0" \
		-s -S \
		-nographic
	while true; do
		clear
		echo "press 'a' to re-make kernel, re-pack and run"
		echo "press 'r' to only re-pack and run"
		echo "press 'o' to only re-run"
		echo "ctrl+c to interrupt"
		read -n 1 choice
		case "$choice" in
			"a")
				REPACK=true
				REMAKE=true
				break
			;;
			"r")
				REPACK=true
				REMAKE=false
				break
			;;
			"o")
				REPACK=false
				REMAKE=false
				break
			;;
			*)
				echo ""
			;;
		esac
	done
	
done
