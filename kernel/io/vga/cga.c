/*
 * Copyright (C) 2026 First Person
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <stdint.h>
#include <stdbool.h>
#include <cga.h>
#include <kernel.h>

int row = 1, col = 1;
unsigned bgd = 0x0;
void print(const char *str, uint8_t bg, uint8_t fg, bool moveWith) {
    volatile uint16_t *vram = (volatile uint16_t *)CGA_BUFFER;

    for(int i = 0; str[i] != 0; i++) {
        if(str[i] == '\n') {
            for(int c = col; c <= CGA_WIDTH; c++)
                vram[(row - 1) * CGA_WIDTH + (c - 1)] = ' ' | ((0x0 & 0x0F) << 8) | ((bgd & 0x0F) << 12);
            row++;
            col = 1;
        } else {
            vram[(row - 1) * CGA_WIDTH + (col - 1)] = str[i] | ((fg & 0x0F) << 8) | ((bg & 0x0F) << 12);
            col++;
            if(col > CGA_WIDTH) {
                col = 1;
                row++;
            }
        }

        if(row > CGA_HEIGHT) {
            for(int r = 0; r < CGA_HEIGHT - 1; r++)
                for(int c = 0; c < CGA_WIDTH; c++)
                    vram[r * CGA_WIDTH + c] = vram[(r + 1) * CGA_WIDTH + c];
            for(int c = 0; c < CGA_WIDTH; c++)
                vram[(CGA_HEIGHT - 1) * CGA_WIDTH + c] = ' ' | ((0x0 & 0x0F) << 8) | ((bgd & 0x0F) << 12);
            row = CGA_HEIGHT;
        }
    }

    if(moveWith)
        moveCursor(col - 1, row - 1);
}

void fillBG(uint8_t bg) {
    volatile uint16_t *vram = (volatile uint16_t *)CGA_BUFFER;

    for(int i = 0; i < CGA_WIDTH * CGA_HEIGHT; i++) {
        vram[i] = ' ' | ((CGA_COLOR_BLACK & 0x0F) << 8) | ((bg & 0x0F) << 12);
    }
    bgd = bg;
    row = 1;
    col = 1;
}

unsigned int getBGColor() {
    return bgd;
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
