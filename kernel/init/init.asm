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
extern kinit
_start:
    cli
    call kinit
    hlt

inb:
    mov dx, di
    in ax, dl
    ret

outb:
    mov al, sil
    mov dx, di
    out dx, al
    ret

section .note.GNU-stack noalloc noexec nowrite progbits