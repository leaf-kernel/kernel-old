#ifndef __CPU_H__
#define __CPU_H__

#include <arch/idt/idt.h>

void hcf();
void hlt();
void panic(const char *reason, int_frame_t frame);

#endif // __CPU_H__