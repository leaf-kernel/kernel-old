#ifndef __CPU_H__
#define __CPU_H__

#include <arch/idt/idt.h>
#include <drivers/stdio/printf.h>
#include <stdbool.h>

void hcf();
void hlt();
void panic(const char *reason, int_frame_t frame);
void cpuid(uint32_t code, uint32_t *a, uint32_t *d);

// MSR
bool cpuHasMSR();
void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi);
void cpuSetMSR(uint32_t msr, uint32_t lo, uint32_t hi);

#endif // __CPU_H__