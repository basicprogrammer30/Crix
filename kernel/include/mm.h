/*
 * Copyright (C) 2026 First Person
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef MM_H
#define MM_H
#include <stdint.h>

typedef enum {
    MEMORY_TYPE_AVAILABLE = 1,
    MEMORY_TYPE_RESERVED  = 2,
    MEMORY_TYPE_ACPI_RECLAIMABLE = 3,
    MEMORY_TYPE_ACPI_NVS = 4,
    MEMORY_TYPE_BAD_MEMORY = 5
} MEMORY_TYPE;

typedef struct MEMORY_REGION {
    uint64_t startAddress;
    uint64_t length;
    uint64_t addr;
    MEMORY_TYPE type;
} MEMORY_REGION;

typedef struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;

    uint32_t mmap_length;
    uint32_t mmap_addr;
} multiboot_info_t;

typedef struct multiboot_mmap_entry {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type;
} mmap_entry_t;

extern void mm_init(multiboot_info_t *mbi, MEMORY_REGION *regions);
extern uint32_t pmm_alloc_frame();
extern void pmm_free_frame(void* addr);
extern uint32_t kmalloc(uint32_t size, int align, uint32_t *paddr);
extern void* kfree(uint32_t *paddr);
extern void map_page(uint32_t vaddr, uint32_t paddr);
extern void *memset(void *dest, int val, size_t leng);
extern void reload_page_directory(uint32_t pd);

#endif // MM_H