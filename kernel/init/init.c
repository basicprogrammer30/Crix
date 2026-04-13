/*
 * Copyright (C) 2026 First Person
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <scheduler.h>
#include <stdint.h>
#include <cga.h>
#include <mm.h>

MEMORY_REGION mregions[32];

void PANIC(char *expection) {
    print(expection, CGA_COLOR_BLACK, CGA_COLOR_RED);
    asm ("cli");
    while(1);
}

void WARN(char *message) {
    print(message, CGA_COLOR_BLACK, CGA_COLOR_YELLOW);
    while(1);
}

void INFO(char *message) {
    print(message, CGA_COLOR_BLACK, CGA_COLOR_GREEN);
    while(1);
}

void kinit(uint32_t magic, uint32_t addr) {
    multiboot_info_t *mbi = (multiboot_info_t*)addr;
    fillBG(CGA_COLOR_BLACK);
    mm_init(mbi, mregions);
    addQueue(print("Doing task 1.\n"));
    addQueue(print("Doing task 2.\n"));
    shd_init();
}
