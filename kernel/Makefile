ifeq ($(DEBUG),1)
CFLAGS  = -ffreestanding -nostdlib -m32 -Iinclude -g -DDEBUG -O0
else
CFLAGS  = -ffreestanding -nostdlib -m32 -Iinclude -O2
endif
ASFLAGS = -f elf32
OBJ =

include init/Makefile
include io/vga/Makefile
include mm/Makefile
include cpu/Makefile

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
