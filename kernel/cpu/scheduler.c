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
#include <string.h>
#include <kernel.h>
#include <sheduler.h>
#include <definations.h>

process_t cprocess;
process_t pqueue[MAX_PROCESSQ];

void shd_init() {
    bool isAllFN_Exists = true;
    for(int i = 0; i < sizeof(pqueue); i++) {
        if(pqueue[i].callback == NULL) {
            isAllFN_Exists = false;
            break;
        }
    }

    if(!isAllFN_Exists)
        PANIC("No Callback Function To Process.");
    INFO("Scheduler Is Initialized Successfully");

    int curidx = 0;
    while(true) {
        if(!(curidx < MAX_PROCESSQ))
            curidx = 0;
        
        cprocess = pqueue[curidx];

        if(!((curidx + 1) < MAX_PROCESSQ))
            cprocess->next = pqueue[curidx + 1];

        switchTask(pqueue[curidx]->regs);
    }
}

int addQueue(void (*callback)()) {
    if (sizeof(pqueue) >= MAX_PROCESSQ)
        return 1;

    // Check If Callback Function Exists in pqueue or not
    bool isFN_Exists = false;
    for(int i = 0; i < sizeof(pqueue); i++) {
        if(pqueue[i].callback == callback) {
            isFN_Exists = true;
            break;
        }
    }

    // Prevent From Callback Function Duplication Into pqueue Variable
    if(isFN_Exists)
        return 2;

    // Using 0 for alignment and NULL for physical address as per your kmalloc signature
    process_t* new_proc = (process_t*)kmalloc(sizeof(process_t), 0, NULL);
    new_proc->regs = (registers_t*)kmalloc(sizeof(registers_t), 0, NULL);
    
    // We use alignment=1 (true) for stack pages usually.
    uint32_t stack_size = 4096;
    void* stack_mem = kmalloc(stack_size, 1, NULL);    
    memset(new_proc->regs, 0, sizeof(registers_t));
    
    uint32_t stack_top = (uint32_t)stack_mem + stack_size;
    
    new_proc->regs->esp = stack_top;
    new_proc->regs->ebp = stack_top; // Standard base pointer init
    new_proc->callback = callback;
    new_proc->next = NULL;

    pqueue[proc_count++] = new_proc;
    if (cprocess == NULL) {
        cprocess = new_proc;
    }

    return 0;
}

int removeQueue(void (*callback)()) {
    // Prevent From Remove Progress Queue If Progress Queue Count Is 1
    int pqcounts = 0;
    for(int i = 0; i < sizeof(pqueue); i++) {
        if(pqueue[i].callback != NULL)
            pqcounts++;
        else
            break;
    }

    if(pqcounts < 2)
        return 1;

    for(int i = 0; i < sizeof(pqueue); i++) {
        if(pqueue[i].callback == callback) {
            pqueue[i] = NULL;
            break;
        }
    }

    return 0;
}

volatile void switchTask(registers_t *regs) {
    // Skip Empty Queue
    if(pqueue.callback == NULL) return;

    // Save Registers Of Current Progress
    memcpy(cprocess->regs, regs, sizeof(registers_t))

    // Do Another Task
    if(cprocess->next) cprocess = cprocess->next;
    else
        if(!((getIndexOfProcess(cprocess) + 1) < MAX_PROCESSQ))
            cprocess = pqueue[getIndexOfProcess(cprocess) + 1];
        else
            cprocess = pqueue[0];
    
    // Now Set Process Registers
    memcpy(regs, cprocess->regs, sizeof(registers_t));

    asm volatile (
        "movl %[new_esp], %%esp\n\t" 
        "movl %[new_eax], %%eax\n\t"
        "movl %[new_ebx], %%ebx\n\t"
        "movl %[new_ecx], %%ecx\n\t"
        "movl %[new_edx], %%edx\n\t"
        "movl %[new_esi], %%esi\n\t"
        "movl %[new_edi], %%edi\n\t"
        "movl %[new_ebp], %%ebp\n\t"

        "call *%[cb]"
        
        : // No outputs
        : [new_esp] "m" (cprocess->regs->esp), // "m" means find it in memory
        [new_eax] "m" (cprocess->regs->eax),
        [new_ebx] "m" (cprocess->regs->ebx),
        [new_ecx] "m" (cprocess->regs->ecx),
        [new_edx] "m" (cprocess->regs->edx),
        [new_esi] "m" (cprocess->regs->esi),
        [new_edi] "m" (cprocess->regs->edi),
        [new_ebp] "m" (cprocess->regs->ebp),
        [cb]      "r" (cprocess->callback)    // "r" puts the function address in a register
        : "memory"
    );
}

int getIndexOfProcess(process_t *proc) {
    int result = 0;
    int i = 0;
    while(1) {
        if(pqueue[i]->callback == proc->callback)
            break;
        i++;
    }
}

process_t getCurrentProcess() {
    return cprocess;
}
