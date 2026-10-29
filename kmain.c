/* kmain.c — Fungsi C utama kernel (x86_64) */

#include "io.h"
#include "console.h"
#include "idt.h"

void kmain64(u64 magic, u64 mbi_addr) {
    (void) magic;
    (void) mbi_addr;

    console_init();
    idt_init();

    serial_print("Hello, OS! (serial)\n");
    serial_print("Kernel pertama berhasil boot.\n");
    serial_print("IDT terpasang. Menguji exception...\n");

    vga_print("Hello, OS!\n");
    vga_print("Kernel pertama berhasil boot.\n");
    vga_print("IDT terpasang. Menguji exception...\n");

    /* Sengaja memicu breakpoint (int 3) untuk menguji IDT */
    __asm__ volatile ("int $3");

    /* Kalau sampai sini, IDT tidak menangani int 3 dengan benar */
    serial_print("TIDAK seharusnya sampai sini.\n");
    vga_print("TIDAK seharusnya sampai sini.\n");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
