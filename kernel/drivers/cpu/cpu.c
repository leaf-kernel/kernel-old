#include <include/cpu.h>

void hcf() {
    asm ("cli");
    hlt();
}

void hlt() {
    for (;;) {
        asm ("hlt");
    }
}