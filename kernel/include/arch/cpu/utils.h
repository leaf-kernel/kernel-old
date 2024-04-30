#ifndef __CPU_UTILS_H__
#define __CPU_UTILS_H__

#include <stdint.h>

#define _int(x) __asm__("int %0" ::"N"(x))
void enable_interrupts();
void disable_interrupts();
unsigned long long read_tsc();
unsigned long read_cr0();
void _get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi);
void _set_msr(uint32_t msr, uint32_t lo, uint32_t hi);

#endif // __CPU_UTILS_H__
