#!/usr/bin/env bash

IMG=build/main_floppy.img

usage() {
    echo "Usage: $0 [--curses | --window]"
    echo "  --curses  Use terminal/curses display"
    echo "  --window  Use a graphical window (default)"
    exit 1
}

DISPLAY_MODE="window"

# Parse args
case "$1" in
    --curses) DISPLAY_MODE="curses" ;;
    --window|"") DISPLAY_MODE="window" ;;
    *) usage ;;
esac

if [ ! -f "$IMG" ]; then
    echo "Image not found: $IMG"
    exit 1
fi

run_curses() {
    echo "Starting QEMU with curses display..."
    qemu-system-i386 -drive file="$IMG",format=raw,if=floppy -display curses
}

run_window() {
    echo "Trying graphical window display..."
    # Try GTK, then SDL as fallback
    qemu-system-i386 -drive file="$IMG",format=raw,if=floppy -display gtk || {
        echo "GTK failed; trying SDL..."
        qemu-system-i386 -drive file="$IMG",format=raw,if=floppy -display sdl
    }
}

run_headless() {
    echo "Falling back to nographic..."
    qemu-system-i386 -drive file="$IMG",format=raw,if=floppy -nographic
}

# Main logic
if [ "$DISPLAY_MODE" = "curses" ]; then
    run_curses || run_headless
else
    run_window || run_headless
fi
