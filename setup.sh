#!/bin/bash
set -e

DISK=/dev/sda   # Replace with your target disk
BOOT_SIZE=1MiB  # Adjust boot partition size as needed

echo "Unmounting any mounted partitions..."
sudo umount ${DISK}?* || true

echo "Wiping first megabyte of disk..."
sudo dd if=/dev/zero of=$DISK bs=1M count=1

echo "Creating partition table..."
sudo parted -s $DISK mklabel msdos

echo "Creating primary partition..."
# Create a primary partition, leave filesystem empty
sudo parted -s $DISK mkpart primary 1MiB 100MiB

echo "Formatting partition as FAT12..."
# Usually the first partition is /dev/sdX1
sudo mkfs.fat -F32 ${DISK}1

echo "Done!"