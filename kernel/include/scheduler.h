/*
 * Copyright (C) 2026 First Person
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef SHEDULER_H
#define SHEDULER_H
#include <stdint.h>

typedef struct {
    uint32_t entries[1024];
} pagedir_t;

typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, ebp, esp;
} registers_t;

typedef struct process_st {
    registers_t *regs;
    pagedir_t   *cr3;
    struct process_st *next;
    void (*callback)();
} process_t;

extern void shd_init();
extern int addQueue(void (*callback)());
extern int removeQueue(void (*callback)());
extern process_t* getCurrentProcess();

#endif // SHEDULER_H
