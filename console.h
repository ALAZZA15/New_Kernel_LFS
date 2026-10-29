#ifndef CONSOLE_H
#define CONSOLE_H

#include "io.h"

void console_init(void);

void vga_putc(char c);
void vga_print(const char *s);

void serial_putc(char c);
void serial_print(const char *s);

void print_hex(u64 val);
void print_dec(u64 val);

#endif
