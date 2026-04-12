;
; Copyright (C) 2026 First Person
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;

bits 32

MB_MAGIC        equ 0x1BADB002
MB_FLAGS        equ 0x00
MB_CHECKSUM     equ - (MB_MAGIC + MB_FLAGS)

section .text
align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

global _start
global inb
global outb
extern kinit
_start:
    cli
    push ebx
    push eax

    call kinit
    hlt

inb:
    push ebp
    mov ebp, esp
    mov dx, [ebp + 8]
    xor eax, eax
    in al, dx
    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp
    mov dx, [ebp + 8]
    mov al, [ebp + 12]
    out dx, al
    pop ebp
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
