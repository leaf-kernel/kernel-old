#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

// Serial port base address for QEMU
#define QEMU_SERIAL_PORT 0x3f8

// Functions for I/O operations
void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
void outd(uint16_t port, uint32_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t ind(uint16_t port);
void iowait(void);

#endif // SERIAL_H
