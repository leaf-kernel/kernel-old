#ifndef __CPU_H__
#define __CPU_H__

#include <arch/cpu/cpuid.h>
#include <arch/x86_64/idt/idt.h>
#include <libc/stdio/printf.h>
#include <libc/stdlib/memory/pmm.h>
#include <libc/string.h>

#include <stdbool.h>

// Standard header
struct ACPISDTHeader {
    char Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
} __attribute__((packed));
;

// CPU utils
void hcf();
void hlt();
void panic(const char *reason, const char *description, int_frame_t frame,
           void *rbp);

// Util funcs (yet again)
void get_cpu_vendor_string(char *vendor_string);
char *get_intel_brand();
int get_model();

// Check functions
bool check_feature(uint32_t feat);
bool check_apic();
bool check_msr();
void check_all_features();

// MSR
void get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi);
void set_msr(uint32_t msr, uint32_t lo, uint32_t hi);

// Register functions
uint64_t _read_reg();
void _write_reg();

#endif  // __CPU_H__
