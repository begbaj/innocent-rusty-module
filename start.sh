#!/bin/bash
source env

toggle() {
  local -n var=$1 # Use nameref (Bash 4.3+)
  if [ "$var" == true ]; then
    var=false
  else
    var=true
  fi
}

clear
NOGRAPH=false
USEGDB=false
REPACK=true
REMAKE=true
while true; do
  while true; do
    clear
    echo "press 'g' to toggle nographic mode ($NOGRAPH) [doesn't work!]"
    echo "press 'd' to toggle gdb mode ($USEGDB)"
    echo "rerun config --------------------"
    echo "press 'a' to re-make kernel, re-pack and run"
    echo "press 'r' to only re-pack and run"
    echo "press 'o' to only re-run"
    echo -e "\033[1;34m !!REMEMBER!! to restart qemu after its execution, type [C-a, x]\033[0m"
    echo "ctrl+c to interrupt"
    read -n 1 choice
    case "$choice" in
    "g")
      toggle NOGRAPH
      continue
      ;;
    "d")
      toggle USEGDB
      continue
      ;;
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

  if $REMAKE; then
    cd linux-$LINUX_VERSION/
    make LLVM=../llvm -j$(nproc)
    cd ..
  fi

  if $REPACK; then
    bash packrootfs.sh
  fi

  clear

  if [ "$USEGDB" == "true" ]; then
    echo -e "\033[1;32mrun gdb and type 'target remote :1234'\033[0m"
    echo -e "\033[1;34m !!REMEMBER!! to restart qemu, type [C-a, x]\033[0m"
  fi

  qemu-system-x86_64 \
    -M pc \
    -m 512M \
    -serial mon:stdio \
    -serial pty \
    -kernel linux-$LINUX_VERSION/arch/x86_64/boot/bzImage \
    -initrd rootfs.cpio.gz \
    -enable-kvm \
    -append "console=ttyS0,115200 console=ttyS1,115200" \
    $([ "$USEGDB" == "true" ] && echo "-s -S") \
    $([ "$NOGRAPH" == "true" ] && echo "-nographic")

done
