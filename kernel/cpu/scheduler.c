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
#include <kernel.h>
#include <scheduler.h>
#include <definations.h>

static process_t *cprocess;
static process_t *pqueue[MAX_PROCESSQ];
static int proc_count;
static volatile void switchTask(registers_t *regs);
static int getIndexOfProcess(process_t *proc);

void shd_init() {
    int qcounts = 0;
    for(int i = 0; i < MAX_PROCESSQ; i++) {
        if(pqueue[i] != 0 || pqueue[i]->callback != 0)
            qcounts++;
    }

    if(!qcounts)
        PANIC("No Callback Function To Process.");
    INFO("Scheduler Is Initialized Successfully");

    int curidx = 0;
    while(true) {
        if(!(curidx < MAX_PROCESSQ))
            curidx = 0;
        
        cprocess = pqueue[curidx];

        if((curidx + 1) < MAX_PROCESSQ)
            cprocess->next = pqueue[curidx + 1];

        switchTask(pqueue[curidx]->regs);
        curidx++;
    }
}

int addQueue(void (*callback)()) {
    if (proc_count >= MAX_PROCESSQ)
        return 1;

    // Check If Callback Function Exists in pqueue or not
    bool isFN_Exists = false;
    for(int i = 0; i < MAX_PROCESSQ; i++) {
        if(pqueue[i] && pqueue[i]->callback == callback) {
            isFN_Exists = true;
            break;
        }
    }

    // Prevent From Callback Function Duplication Into pqueue Variable
    if(isFN_Exists)
        return 2;

    // Using 0 for alignment and 0 for physical address as per your kmalloc signature
    process_t* new_proc = (process_t*)kmalloc(sizeof(process_t), 0, 0);
    new_proc->regs = (registers_t*)kmalloc(sizeof(registers_t), 0, 0);
    
    // We use alignment=1 (true) for stack pages usually.
    uint32_t stack_size = 4096;
    void* stack_mem = kmalloc(stack_size, 1, 0);    
    memset(new_proc->regs, 0, sizeof(registers_t));
    
    uint32_t stack_top = (uint32_t)stack_mem + stack_size;
    
    new_proc->regs->esp = stack_top;
    new_proc->regs->ebp = stack_top; // Standard base pointer init
    new_proc->callback = callback;
    new_proc->next = 0;

    pqueue[proc_count++] = new_proc;
    if (cprocess == 0) {
        cprocess = new_proc;
    }

    return 0;
}

int removeQueue(void (*callback)()) {
    // Prevent From Remove Progress Queue If Progress Queue Count Is 1
    int pqcounts = 0;
    for(int i = 0; i < MAX_PROCESSQ; i++) {
        if(pqueue[i] && pqueue[i]->callback != 0)
            pqcounts++;
        else
            break;
    }

    if(pqcounts < 2)
        return 1;

    for(int i = 0; i < MAX_PROCESSQ; i++) {
        if(pqueue[i] && pqueue[i]->callback == callback) {
            pqueue[i] = 0;
            break;
        }
    }

    return 0;
}

static volatile void switchTask(registers_t *regs) {
    // Skip Empty Queue
    if(cprocess == 0) return;

    // Save Registers Of Current Progress
    memcpy(cprocess->regs, regs, sizeof(registers_t));

    // Do Another Task
    if(cprocess->next) cprocess = cprocess->next;
    else
        if(!((getIndexOfProcess(cprocess) + 1) < MAX_PROCESSQ))
            cprocess = pqueue[getIndexOfProcess(cprocess) + 1];
        else
            cprocess = pqueue[0];
    
    // Now Set Process Registers
    memcpy(regs, cprocess->regs, sizeof(registers_t));

    cprocess->callback();
}

static int getIndexOfProcess(process_t *proc) {
    for(int i = 0; i < MAX_PROCESSQ; i++) {
        if(pqueue[i] && pqueue[i]->callback == proc->callback)
            return i;
    }
    return -1;
}

process_t* getCurrentProcess() {
    return cprocess;
}
