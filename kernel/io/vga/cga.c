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
#include <kernel.h>

void print(const char *str, uint8_t bg, uint8_t fg) {
    volatile uint16_t *vram = (volatile uint16_t *)CGA_BUFFER;

    for(int i = 0; str[i] != 0; i++) {
        vram[i] = str[i] | ((fg & 0x0F) << 8) | ((bg & 0x0F) << 12);
    }
}

void fillBG(uint8_t bg) {
    volatile uint16_t *vram = (volatile uint16_t *)CGA_BUFFER;

    for(int i = 0; i < CGA_WIDTH * CGA_HEIGHT; i++) {
        vram[i] = ' ' | ((CGA_COLOR_BLACK & 0x0F) << 8) | ((bg & 0x0F) << 12);
    }
}

void enableCorsor(uint8_t cursorStart, uint8_t cursorEnd) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursorStart);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursorEnd);
}

void disableCursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void moveCursor(uint8_t x, uint8_t y) {
    uint16_t pos = y * CGA_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);

    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
}
