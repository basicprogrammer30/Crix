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
#include <stddef.h>
#include <kernel.h>
#include <mm.h>

#define PAGE_PRESENT 0x1
#define PAGE_WRITE   0x2

#define FRAME_SIZE 4096
#define MMNINIT "Memory Manager Is Not Initialized."

typedef uint32_t page_t;
static uint32_t *frame_bitmap = (uint32_t*)&kend;
static uint32_t total_frames;
static uint32_t heapStart = 0, heapEnd = 0;
page_t *page_directory = 0; // Just after bitmap, adjust if needed

bool mmIsInitalized = false;

static inline void set_frame(uint32_t frame);
static inline void clear_frame(uint32_t frame);
static inline bool test_frame(uint32_t frame);

void mm_init(multiboot_info_t *mbi, MEMORY_REGION *regions) {
    if (mmIsInitalized)
        return;

    uint32_t total_mem = (mbi->mem_upper + 1024) * 1024;
    total_frames = total_mem / FRAME_SIZE;

    uint32_t bitmap_size = total_frames / 8;

    // Align bitmap after kernel
    uint32_t addr = (uint32_t)&kend;
    addr = (addr + FRAME_SIZE - 1) & ~(FRAME_SIZE - 1);
    frame_bitmap = (uint32_t*)addr;

    // Allocate page directory
    page_directory = (page_t*)pmm_alloc_frame();
    memset(page_directory, 0, FRAME_SIZE);

    heapStart = addr;
    heapEnd = addr;

    // Mark ALL frames as used
    for (uint32_t i = 0; i < bitmap_size / 4; i++) {
        frame_bitmap[i] = 0xFFFFFFFF;
    }

    // parse mmap and FREE usable memory
    uint32_t current = mbi->mmap_addr;
    uint32_t mmap_end = mbi->mmap_addr + mbi->mmap_length;

    while (current < mmap_end) {
        mmap_entry_t *entry = (mmap_entry_t*)current;

        if (entry->type == MEMORY_TYPE_AVAILABLE) {
            uint64_t start = entry->addr;
            uint64_t end   = entry->addr + entry->len;

            for (uint64_t a = start; a < end; a += FRAME_SIZE) {
                clear_frame(a / FRAME_SIZE);
            }
        }

        current += entry->size + sizeof(entry->size);
    }

    // reserve kernel + bitmap
    uint32_t kernel_start = 0x100000; // adjust if needed
    uint32_t kernel_end   = addr + bitmap_size;

    for (uint32_t a = kernel_start; a < kernel_end; a += FRAME_SIZE) {
        set_frame(a / FRAME_SIZE);
    }

    mmIsInitalized = true;
    INFO("Memory Manager is Initialized Successfully.");
}

static inline void set_frame(uint32_t frame) {
    frame_bitmap[frame / 32] |= (1 << (frame % 32));
}

static inline void clear_frame(uint32_t frame) {
    frame_bitmap[frame / 32] &= ~(1 << (frame % 32));
}

static inline bool test_frame(uint32_t frame) {
    if(!mmIsInitalized)
        PANIC(MMNINIT);
    return frame_bitmap[frame / 32] & (1 << (frame % 32));
}

uint32_t pmm_alloc_frame() {
    for (uint32_t i = 0; i < total_frames / 32; i++) {
        if(frame_bitmap[i] != 0xFFFFFFFF) {
            for (uint32_t j = 0; j < 32; j++) {
                if ((frame_bitmap[i] & (1 << j)) == 0) {
                    set_frame(i * 32 + j);
                    return ((i * 32 + j) * FRAME_SIZE);
                }
            }
        }
        else
            return 0;
    }
}

void pmm_free_frame(void* addr) {
    uint32_t frame = (uint32_t)addr / FRAME_SIZE;
    clear_frame(frame);
}

uint32_t kmalloc(uint32_t size, int align, uint32_t *paddr) {
    if(!mmIsInitalized)
        PANIC(MMNINIT);
    
    // Align if heap requested.
    if (align && (heapEnd & (FRAME_SIZE - 1))) {
        heapEnd = (heapEnd + FRAME_SIZE) & ~(FRAME_SIZE - 1);
    }

    uint32_t addr = heapEnd;
    heapEnd += size;

    // Allocate physcal frames if requested
    for(uint32_t a = addr; a < heapEnd; a += FRAME_SIZE) {
        uint32_t padr = pmm_alloc_frame();
        map_page(padr, a);
    }

    // Return physcal address if requested
    if(paddr)
        *paddr = addr;
    
    return addr;
}

uint32_t virt_to_phys(uint32_t vaddr) {
    if(!mmIsInitalized)
        PANIC(MMNINIT);

    uint32_t pd_idx = vaddr >> 22;
    uint32_t *pt_idx = (vaddr >> 12) & 0x3FF;

    page_t table = (page_t*)(page_directory[pd_idx] * FRAME_SIZE);
    
    // Check if page is exist and mapped
    if((page_directory[pd_idx] & PAGE_PRESENT) == 0 || (table[pt_idx] & PAGE_PRESENT) == 0)
        return 0; // Not mapped

    return (table[pt_idx] & 0xFFF) | (vaddr & 0xFFF);
}

void* kfree(uint32_t *paddr) {
    if(!paddr)
        return;
    
    if((uint32_t)paddr == heapEnd)
        heapEnd = (uint32_t)paddr;
}

void map_page(uint32_t vaddr, uint32_t paddr) {
    if(!mmIsInitalized)
        PANIC(MMNINIT);

    uint32_t pd_idx = vaddr >> 22;
    uint32_t *pt_idx = (vaddr >> 12) & 0x3FF;

    page_t table = (page_t*)(page_directory[pd_idx] * FRAME_SIZE);
    
    // If page table doesn't exist, create it
    if(!table) {
        table = (page_t*)pmm_alloc_frame();
        memset(table, 0, FRAME_SIZE);
        page_directory[pd_idx] = (uint32_t)table | PAGE_PRESENT | PAGE_WRITE;
    }

    table[pt_idx] = (paddr & 0xFFFFF000) | PAGE_PRESENT | PAGE_WRITE;
}

void *memset(void *dest, int val, size_t leng) {
    uint8_t *ptr = (uint8_t*)dest;
    for (size_t i = 0; i < leng; i++) {
        ptr[i] = (uint8_t)val;
    }
    return dest;
}

void reload_page_directory(uint32_t pd) {
    asm volatile ("mov %0, %%cr3" :: "r"(pd));
}
