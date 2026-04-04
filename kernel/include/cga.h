/*
 * Copyright (C) 2026 First Person
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef CGA_H
#define CGA_H
#include <stdint.h>

#define CGA_WIDTH  80
#define CGA_HEIGHT 25
#define CGA_BUFFER 0xB8000

enum {
    CGA_COLOR_BLACK         = 0x0,
    CGA_COLOR_BLUE          = 0x1,
    CGA_COLOR_GREEN         = 0x2,
    CGA_COLOR_CYAN          = 0x3,
    CGA_COLOR_RED           = 0x4,
    CGA_COLOR_MAGENTA       = 0x5,
    CGA_COLOR_BROWN         = 0x6,
    CGA_COLOR_LIGHT_GREY    = 0x7,
    CGA_COLOR_DARK_GREY     = 0x8,
    CGA_COLOR_LIGHT_BLUE    = 0x9,
    CGA_COLOR_LIGHT_GREEN   = 0xA,
    CGA_COLOR_LIGHT_CYAN    = 0xB,
    CGA_COLOR_LIGHT_RED     = 0xC,
    CGA_COLOR_LIGHT_MAGENTA = 0xD,
    CGA_COLOR_YELLOW        = 0xE,
    CGA_COLOR_WHITE         = 0xF
};

extern void print(const char *str, uint8_t bg, uint8_t fg);

#endif // CGA_H