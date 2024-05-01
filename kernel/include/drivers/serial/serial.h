#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>

#include <arch/cpu/cpu.h>
#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

#define _SERIAL_COM1 0x3F8
#define _SERIAL_COM2 0x2F8
#define _SERIAL_COM3 0x3E8
#define _SERIAL_COM4 0x2E8
#define _SERIAL_COM5 0x5F8
#define _SERIAL_COM6 0x4F8
#define _SERIAL_COM7 0x5E8
#define _SERIAL_COM8 0x4E8

// Functions for I/O operations
void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
void outd(uint16_t port, uint32_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t ind(uint16_t port);
void iowait(void);

// Functions for serial (COM)
void init_serial();
void switch_serial(uint8_t id, uint16_t port);
char read_serial();
void write_serial(char a);

extern bool _serial_has_been_init;
extern char *_serial_cur_com_char;

#endif // SERIAL_H
