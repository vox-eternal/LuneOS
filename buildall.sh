#!/usr/bin/env bash

set -e

echo "=== LuneOS Build All ==="
echo ""

# Ensure test.txt exists (for the FAT image)
if [ ! -f test.txt ]; then
    echo "Creating test.txt..."
    echo "Hello from LuneOS!" > test.txt
fi

echo "[1/4] Cleaning old builds..."
make clean
echo "✓ Clean complete"
echo ""

echo "[2/4] Building bootloader and kernel..."
make
echo "✓ Bootloader and kernel built"
echo ""

echo "[3/4] Building FAT tool..."
make tools_fat
echo "✓ FAT tool built"
echo ""

echo "[4/4] Verifying build artifacts..."
if [ -f build/main_floppy.img ]; then
    SIZE=$(stat -f%z build/main_floppy.img 2>/dev/null || stat -c%s build/main_floppy.img 2>/dev/null)
    echo "✓ Floppy image: $(basename build/main_floppy.img) ($SIZE bytes)"
else
    echo "✗ Floppy image not found"
    exit 1
fi

if [ -f build/tools/fat ]; then
    echo "✓ FAT tool: $(basename build/tools/fat)"
else
    echo "✗ FAT tool not found"
    exit 1
fi

echo ""
echo "=== Build Complete ==="
echo ""
echo "Next steps:"
echo "  Run bootloader in QEMU: ./run.sh"
echo "  Extract file from image: ./build/tools/fat build/main_floppy.img <filename>"
echo ""
