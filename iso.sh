#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/jloader isodir/boot/jloader
cp sysroot/boot/initrd.tar isodir/boot/initrd.tar
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "JAOS" {
	multiboot /boot/jloader
	module    /boot/initrd.tar
}
EOF
grub-mkrescue -o jaos.iso isodir
