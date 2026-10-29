# MyOS

A from-scratch x86_64 kernel - not a Linux fork, not a distro. Boots via GRUB Multiboot 1, transitions from 32-bit protected mode to 64-bit long mode, sets up its own GDT and IDT, and outputs to VGA text mode and serial COM1.

## Overview

MyOS is a minimal x86_64 kernel built entirely from scratch - not a Linux fork, not a distribution, not a derivative of any existing project. Every line of code is written by hand: the Multiboot header, the 32-bit to 64-bit long mode transition with its own page tables (PML4, PDPT, PD), the GDT with a far return to reload CS, and an Interrupt Descriptor Table (IDT) with 32 CPU exception stubs.

The goal is to understand how a kernel works at the lowest level: how the CPU is initialized, how memory is mapped, how interrupts are handled, and how the kernel talks to hardware through port I/O. The toolchain itself - `x86_64-elf-gcc` 13.2.0 - was compiled manually from Binutils and GCC sources.

The kernel currently boots via GRUB, enters long mode, prints text to VGA text mode and serial COM1, and catches CPU exceptions - printing the exception name, error code, and RIP to the screen. Next milestones: PIC remap, timer, keyboard driver, memory allocator, and a minimal userspace.

Written in C and x86_64 assembly, built with Makefile, tested in QEMU.

## Features

- Boot via GRUB Multiboot 1
- 32-bit protected mode -> 64-bit long mode transition (PML4 + PDPT + PD, PAE, EFER.LME, paging)
- 64-bit GDT with far return to reload CS
- VGA text mode (80x25) and serial COM1 output
- IDT with 32 CPU exception stubs (0..31)
- Exception handler that prints exception name, error code, and RIP

## Build

Requires `x86_64-elf-gcc`, `nasm`, `qemu-system-x86_64`, `grub-mkrescue`, `xorriso`.

    make iso        # build myos.iso
    make run        # boot in QEMU (graphical window)
    make clean      # clean build artifacts

## Structure

    boot.asm      Multiboot header + 32->64-bit bootstrap + GDT
    linker.ld     Kernel memory layout (loaded at 1 MB)
    grub.cfg      GRUB configuration
    kmain.c       C entry point, initializes console + IDT
    console.c/h   VGA text mode and serial COM1 output
    idt.c/h       Interrupt Descriptor Table (32 entries)
    isr.c/h       CPU exception handler
    isr.asm       32 exception stubs
    io.h          Port I/O helpers (outb/inb)

## Status

Early stage - milestone reached: kernel boots, prints text, and catches exceptions.
Next: PIC remap, timer, keyboard, memory allocator, userspace.

## Documentation

- [Migration guide](docs/migration.md)
