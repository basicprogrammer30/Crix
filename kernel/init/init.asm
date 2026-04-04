;
; Copyright (C) 2026 First Person
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;

bits 32

MB_MAGIC        equ 0x1BAD002
MB_FLAGS        equ 0x0010003
MB_CHECKSUM     equ -(MB_MAGIC + MB_FLAGS)

section .multiboot
align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

global _start
extern kinit
_start:
    cli
    call kinit
.loop:
    jmp .loop

section .note.GNU-stack noalloc noexec nowrite progbits