#include <include/drivers/stdio/serial.h>

#define UNUSED 0x80

void iowait() { outb(UNUSED, 0); }

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

void outw(uint16_t port, uint16_t value) {
    __asm__ volatile("outw %1, %0" : : "dN"(port), "a"(value));
}

void outd(uint16_t port, uint32_t value) {
    __asm__ volatile("outl %1, %0" : : "dN"(port), "a"(value));
}

uint8_t inb(uint16_t port) {
    uint8_t r;
    __asm__ volatile("inb %1, %0" : "=a"(r) : "dN"(port));
    return r;
}

uint16_t inw(uint16_t port) {
    uint16_t r;
    __asm__ volatile("inw %1, %0" : "=a"(r) : "dN"(port));
    return r;
}

uint32_t ind(uint16_t port) {
    uint32_t r;
    __asm__ volatile("inl %1, %0" : "=a"(r) : "dN"(port));
    return r;
}