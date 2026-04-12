CFLAGS  = -ffreestanding -nostdlib -m32 -Iinclude
ASFLAGS = -f elf32
OBJ =

include init/Makefile
include io/vga/Makefile
include mm/Makefile

all: crix

crix: $(OBJ)
	@echo "  LD  $@"
	@ld -m elf_i386 -T linker.ld -o $@ $^

%.o: %.c
	@echo "  CC  $@"
	@$(CC) $(CFLAGS) -c $< -o $@

%.a: %.asm
	@echo "  ASM $@"
	@$(ASM) $(ASFLAGS) $< -o $@

%.a: %.s
	@echo "  AS  $@"
	@$(CC) $(CFLAGS) -c $< -o $@
