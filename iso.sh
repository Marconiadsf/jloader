#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/jloader isodir/boot/jloader

cat > isodir/boot/grub/grub.cfg << EOF
menuentry "JAOS" {
	multiboot /boot/jloader
	
}
EOF
grub-mkrescue -o jaos.iso isodir
