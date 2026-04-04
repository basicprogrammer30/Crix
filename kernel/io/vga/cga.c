/*
 * Copyright (C) 2026 First Person
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <stdint.h>
#include <cga.h>

void print(const char *str, uint8_t bg, uint8_t fg) {
    volatile uint16_t *vram = (volatile uint16_t *)CGA_BUFFER;

    for(int i = 0; str[i] == 0; i++) {
        vram[i] = ((fg & 0x0F) << 4) | ((bg & 0x0F) << 4) | str[i];
    }
}