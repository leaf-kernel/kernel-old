#include <include/drivers/cpu/cpu.h>

void hcf() {
    asm ("cli");
    hlt();
}

void hlt() {
    for (;;) {
        asm ("hlt");
    }
}