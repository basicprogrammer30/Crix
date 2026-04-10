/*
 * Copyright (C) 2026 First Person
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef KERNEL_H
#define KERNEL_H
#include <stdint.h>
#include <cga.h>
#include <mm.h>

extern uint16_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t value);
extern void PANIC(char *expection);
extern void WARN(char *message);
extern void INFO(char *message);

#endif // KERNEL_H