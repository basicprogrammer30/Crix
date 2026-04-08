OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

SECTIONS {
    . = 0x100000;

    .multiboot ALIGN(4) : AT(0x100000)
    {
        KEEP(*(.multiboot))
    }

    .text ALIGN(4) : 
    {
        *(.text)
    }

    .rodata ALIGN(4) :
    {
        *(.rodata)
    }

    .data ALIGN(4) :
    {
        *(.data)
    }

    .bss ALIGN(4) :
    {
        *(COMMON)
        *(.bss)
    }
}