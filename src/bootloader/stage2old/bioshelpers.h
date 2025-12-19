#pragma once
#include "stdint.h"

static inline uint8_t bios_kbhit(void);

static inline uint8_t bios_getch(void);

static inline void bios_putchar(uint8_t c);

void checkKeyStroke(void);