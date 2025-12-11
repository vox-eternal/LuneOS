#!/usr/bin/env bash

# Use an explicit raw format and a terminal (curses) display so this
# script works inside headless containers where GTK/X is unavailable.
IMG=build/main_floppy.img

if [ ! -f "$IMG" ]; then
	echo "Image not found: $IMG"
	exit 1
fi

echo "Trying terminal curses display (if available)..."
qemu-system-i386 -drive file="$IMG",format=raw,if=floppy -display curses || {
	echo "curses display failed or unsupported; trying headless display..."
	qemu-system-i386 -drive file="$IMG",format=raw,if=floppy -display none || {
		echo "fallback to -nographic..."
		qemu-system-i386 -drive file="$IMG",format=raw,if=floppy -nographic || {
			echo "All QEMU display attempts failed or QEMU not present on PATH."
			exit 2
		}
	}
}