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

extern void kend;

void PANIC(char *expection) {
    print(expection + '\n', getBGColor(), CGA_COLOR_RED, true);
    asm ("cli");
    while(1) {
        asm("hlt");
    }
}

void ERROR(char *expection) {
    print(expection + '\n', getBGColor(), CGA_COLOR_RED, true);
}

void WARN(char *message) {
    print(message + '\n', getBGColor(), CGA_COLOR_YELLOW, true);
}

void INFO(char *message) {
    print(message + '\n', getBGColor(), CGA_COLOR_GREEN, true);
}

void kinit(uint32_t magic, uint32_t addr) {
    multiboot_info_t *mbi = (multiboot_info_t*)addr;
    fillBG(CGA_COLOR_BLUE);
    INFO("Starting mm_init");
    mm_init(mbi, (void*)&kend);
    INFO("mm_init returned");
}
