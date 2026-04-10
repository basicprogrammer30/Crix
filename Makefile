CC = gcc
ASM = nasm
TARGET_DIR = target

JUNKS = *.o *.a *.bin *.elf *.log *.img *.iso *.cpio* crix*

.PHONY: build
build:
	@$(MAKE) clean
	@mkdir -p $(TARGET_DIR)/boot/grub
	@$(MAKE) -C kernel
	@cp kernel/crix $(TARGET_DIR)/boot

	@echo set timeout=0 > $(TARGET_DIR)/boot/grub/menu.lst
	@echo menutry ""Crix"" { >> $(TARGET_DIR)/boot/grub/menu.lst
	@echo "	   multiboot /boot/crix" >> $(TARGET_DIR)/boot/grub/menu.lst
	@echo "	   boot" >> $(TARGET_DIR)/boot/grub/menu.lst
	@echo } >> $(TARGET_DIR)/boot/grub/menu.lst
	@grub-mkrescue $(TARGET_DIR) -o crix.iso

clean:
	@$(foreach JUNK, $(JUNKS), $(shell find . -name "$(JUNK)" -delete))
	@rm -rf $(TARGET_DIR)

export CC
export ASM