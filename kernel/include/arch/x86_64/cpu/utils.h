#ifndef __CPU_UTILS_H__
#define __CPU_UTILS_H__

#include <stdint.h>

#define _int(x) __asm__("int %0" ::"N"(x))

static inline void cpuid(int code, uint32_t *a, uint32_t *d) {
    __asm__ volatile("cpuid" : "=a"(*a), "=d"(*d) : "0"(code) : "ebx", "ecx");
}

static inline uint64_t rdtsc(void) {
    uint32_t lo;
    uint32_t hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

static inline void invlpg(void *tlb) {
    __asm__ volatile("invlpg (%0)" ::"b"(tlb) : "memory");
}

////
// Model Specific Registers
///

static inline uint64_t rdmsr(uint64_t msr) {
    uint32_t lo;
    uint32_t hi;
    __asm__ volatile("rdmsr" : "=a"(lo), "=d"(hi) : "c"(msr));
    return ((uint64_t)hi << 32) | lo;
}

static inline void wrmsr(uint64_t msr, uint64_t val) {
    uint32_t lo = val & 0xFFFFFFFF;
    uint32_t hi = val >> 32;
    __asm__ volatile("wrmsr" ::"c"(msr), "a"(lo), "d"(hi));
}

static inline uint64_t read_cr0() {
    uint64_t val;
    __asm__ volatile("mov %%cr0, %0" : "=r"(val));
    return val;
}

static inline uint64_t read_cr2() {
    uint64_t val;
    __asm__ volatile("mov %%cr2, %0" : "=r"(val));
    return val;
}

static inline uint64_t read_cr3() {
    uint64_t val;
    __asm__ volatile("mov %%cr3, %0" : "=r"(val));
    return val;
}

static inline uint64_t read_cr4() {
    uint64_t val;
    __asm__ volatile("mov %%cr4, %0" : "=r"(val));
    return val;
}

static inline uint64_t read_cr8() {
    uint64_t val;
    __asm__ volatile("mov %%cr8, %0" : "=r"(val));
    return val;
}

static inline void write_cr0(uint64_t val) {
    __asm__ volatile("mov %0, %%cr0" ::"r"(val));
}

static inline void write_cr2(uint64_t val) {
    __asm__ volatile("mov %0, %%cr2" ::"r"(val));
}

static inline void write_cr3(uint64_t val) {
    __asm__ volatile("mov %0, %%cr3" ::"r"(val));
}

static inline void write_cr4(uint64_t val) {
    __asm__ volatile("mov %0, %%cr4" ::"r"(val));
}

static inline void write_cr8(uint64_t val) {
    __asm__ volatile("mov %0, %%cr8" ::"r"(val));
}

void enable_interrupts();
void disable_interrupts();
void _shutdown_emu();
void _reboot();

#endif  // __CPU_UTILS_H__
