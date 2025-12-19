#include "bioshelpers.h"
#include "stdio.h"

static inline uint8_t bios_getch(void) {
    uint8_t c;
    __asm {
        mov ah, 00h
        int 16h
        mov c, al     
    }
    return c;
}

void checkKeyStroke(void) {
    while (1) {
        /* wait until key available */
        uint8_t c = bios_getch();
        putc(c);

        if (c == 0x0D) {      // Enter
            putc(0x0D);
            putc(0x0A);
        }
    }
}

