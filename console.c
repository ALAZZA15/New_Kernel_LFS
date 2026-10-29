/* console.c — output VGA text mode + serial COM1 */

#include "console.h"

#define VGA_WIDTH  80
#define VGA_MEMORY ((volatile u16 *) 0xB8000)
#define COM1       0x3F8

static u64 vga_row = 0;
static u64 vga_col = 0;
static u8  vga_color = 0x0A;

/* -------- Serial -------- */
static void serial_init_internal(void) {
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x80);
    outb(COM1 + 0, 0x03);
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);
    outb(COM1 + 2, 0xC7);
    outb(COM1 + 4, 0x0B);
}

static int serial_tx_ready(void) {
    return inb(COM1 + 5) & 0x20;
}

void serial_putc(char c) {
    if (c == '\n') serial_putc('\r');
    while (!serial_tx_ready()) { }
    outb(COM1, (u8) c);
}

void serial_print(const char *s) {
    while (*s) serial_putc(*s++);
}

/* -------- VGA -------- */
void vga_putc(char c) {
    if (c == '\n') { vga_row++; vga_col = 0; return; }
    VGA_MEMORY[vga_row * VGA_WIDTH + vga_col] = ((u16) vga_color << 8) | (u8) c;
    vga_col++;
    if (vga_col >= VGA_WIDTH) { vga_col = 0; vga_row++; }
}

void vga_print(const char *s) {
    while (*s) vga_putc(*s++);
}

/* -------- Helpers -------- */
void print_hex(u64 val) {
    const char *hex = "0123456789ABCDEF";
    char buf[18];
    buf[0] = '0'; buf[1] = 'x';
    for (int i = 0; i < 16; i++) {
        buf[2 + i] = hex[(val >> ((15 - i) * 4)) & 0xF];
    }
    buf[18 - 1] = 0;
    serial_print(buf);
    vga_print(buf);
}

void print_dec(u64 val) {
    char buf[21];
    int i = 20;
    buf[i] = 0;
    if (val == 0) { buf[--i] = '0'; }
    else {
        while (val > 0 && i > 0) {
            buf[--i] = '0' + (val % 10);
            val /= 10;
        }
    }
    serial_print(&buf[i]);
    vga_print(&buf[i]);
}

/* -------- Init -------- */
void console_init(void) {
    serial_init_internal();
}
