/* isr.c — handler exception CPU (0..31) untuk x86_64 */

#include "io.h"
#include "console.h"
#include "isr.h"

static const char *exception_names[32] = {
    "Divide Error",
    "Debug",
    "NMI",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point",
    "Virtualization",
    "Control Protection",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved"
};

void isr_handler(u64 int_no, u64 err_code, u64 rip) {
    serial_print("\n=== EXCEPTION ===\n");
    vga_print("\n=== EXCEPTION ===\n");

    if (int_no < 32) {
        serial_print(exception_names[int_no]);
        vga_print(exception_names[int_no]);
    } else {
        serial_print("Unknown exception ");
        vga_print("Unknown exception ");
        print_dec(int_no);
    }
    serial_print("\n  int_no:   ");
    vga_print("\n  int_no:   ");
    print_dec(int_no);

    serial_print("\n  err_code: ");
    vga_print("\n  err_code: ");
    print_hex(err_code);

    serial_print("\n  rip:      ");
    vga_print("\n  rip:      ");
    print_hex(rip);

    serial_print("\n=================\n");
    vga_print("\n=================\n");

    /* Hang: exception fatal, tidak kembali */
    for (;;) {
        __asm__ volatile ("cli; hlt");
    }
}
