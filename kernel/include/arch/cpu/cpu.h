#ifndef __CPU_H__
#define __CPU_H__

#include <arch/x86_64/idt/idt.h>
#include <arch/cpu/cpuid.h>
#include <libc/string.h>
#include <libc/stdio/printf.h>
#include <libc/stdlib/memory/pmm.h>

#include <stdbool.h>

// CPU utils
void hcf();
void hlt();
void panic(const char *reason, const char *description, int_frame_t frame);

// Util funcs (yet again)
void get_intel_cpu_brand_string(char *brand_string);
void get_cpu_vendor_string(char *vendor_string);
int get_model();

// Check functions
bool check_feature(uint32_t feat);
bool check_apic();
bool check_msr();
void check_all_features();

// MSR
void get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi);
void set_msr(uint32_t msr, uint32_t lo, uint32_t hi);

#endif // __CPU_H__
