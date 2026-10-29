; boot.asm — Multiboot 1 → 32-bit protected → long mode (x86_64)

MB_MAGIC    equ 0x1BADB002
MB_FLAGS    equ 0x00000000
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

section .multiboot
align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

section .bss
align 4096
pml4:       resb 4096
pdpt:       resb 4096
pd:         resb 4096
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
BITS 32
global _start
extern kmain64

_start:
    cli
    mov [saved_magic], eax
    mov [saved_mbi],   ebx

    mov edi, pml4
    xor eax, eax
    mov ecx, 4096/4
    rep stosd
    mov edi, pdpt
    mov ecx, 4096/4
    rep stosd
    mov edi, pd
    mov ecx, 4096/4
    rep stosd

    mov dword [pml4], pdpt + 0x03
    mov dword [pml4 + 4], 0
    mov dword [pdpt], pd + 0x03
    mov dword [pdpt + 4], 0

    mov edi, pd
    mov eax, 0x00000083
    mov ecx, 512
.fill_pd:
    mov [edi], eax
    mov dword [edi + 4], 0
    add eax, 0x00200000
    add edi, 8
    loop .fill_pd

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov eax, pml4
    mov cr3, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    lgdt [gdt64_pointer]
    jmp 0x08:long_mode_entry

BITS 64
long_mode_entry:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rsp, stack_top
    mov rbp, rsp

    mov edi, [saved_magic]
    mov esi, [saved_mbi]
    call kmain64

.hang:
    cli
    hlt
    jmp .hang

section .data
align 8
saved_magic: dd 0
saved_mbi:   dd 0

align 16
gdt64_start:
    dq 0x0000000000000000
gdt64_code:
    dq 0x00209A0000000000
gdt64_data:
    dq 0x0000000000009200
gdt64_end:

gdt64_pointer:
    dw gdt64_end - gdt64_start - 1
    dq gdt64_start
