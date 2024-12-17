cd rootfs
echo "_________________"
echo "packing rootfs..."
find . -print0 | cpio --null -ov --format=newc | gzip -9 -q | tee ../rootfs.cpio.gz >/dev/null
echo "  packing done"
echo "_________________"
