#ifndef __CPU_H__
#define __CPU_H__

#include <arch/idt/idt.h>
#include <drivers/stdio/printf.h>
#include <memory/pmm.h>

#include <stdbool.h>
#include <cpuid.h>

// CPU utils
void hcf();
void hlt();
void panic(const char *reason, int_frame_t frame);

// CPUID Stuff
enum cpuid_requests
{
    CPUID_GETVENDORSTRING,
    CPUID_GETFEATURES,
    CPUID_GETTLB,
    CPUID_GETSERIAL,

    CPUID_INTELEXTENDED = 0x80000000,
    CPUID_INTELFEATURES,
    CPUID_INTELBRANDSTRING,
    CPUID_INTELBRANDSTRINGMORE,
    CPUID_INTELBRANDSTRINGEND,
};

int get_model();
int cpuid_string(int code, uint32_t where[4]);

// Util funcs (yet again)
void get_intel_cpu_brand_string(char *brand_string);
void get_cpu_vendor_string(char *vendor_string);

#endif // __CPU_H__