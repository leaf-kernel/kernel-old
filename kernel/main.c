#include <include/serial.h>

static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

void _start(void) {
    outb(QEMU_SERIAL_PORT, 'A');
    hcf();
}