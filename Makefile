# Makefile — MyOS

CROSS   := $(HOME)/opt/cross/bin
CC      := $(CROSS)/x86_64-elf-gcc
LD      := $(CROSS)/x86_64-elf-ld
ASM     := nasm

CFLAGS  := -ffreestanding -fno-stack-protector -fno-pic -mno-red-zone \
           -mno-sse -mno-sse2 -mno-mmx -m64 -Wall -Wextra -O2 \
           -fno-builtin -nostdlib -nostdinc
ASMFLAGS := -f elf64
LDFLAGS  := -T linker.ld -nostdlib -static

OBJS    := boot.o isr.o console.o idt.o isr_c.o kmain.o
KERNEL  := myos.bin
ISO     := myos.iso

.PHONY: all clean run run-serial iso

all: $(KERNEL)

boot.o: boot.asm
	$(ASM) $(ASMFLAGS) $< -o $@

isr.o: isr.asm
	$(ASM) $(ASMFLAGS) $< -o $@

console.o: console.c console.h io.h
	$(CC) $(CFLAGS) -c $< -o $@

idt.o: idt.c idt.h isr.h io.h
	$(CC) $(CFLAGS) -c $< -o $@

isr_c.o: isr.c isr.h console.h io.h
	$(CC) $(CFLAGS) -c $< -o $@

kmain.o: kmain.c console.h idt.h io.h
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

iso: $(ISO)

$(ISO): $(KERNEL) grub.cfg
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/myos.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso

run: $(ISO)
	qemu-system-x86_64 -cdrom $(ISO) -m 256M -serial stdio

run-serial: $(ISO)
	qemu-system-x86_64 -cdrom $(ISO) -m 256M -display none -serial stdio -no-reboot

clean:
	rm -f *.o $(KERNEL) $(ISO)
	rm -rf iso
