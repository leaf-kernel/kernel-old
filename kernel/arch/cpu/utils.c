#include <arch/cpu/utils.h>

void disable_interrupts()
{
    __asm__ volatile("cli" : : : "memory");
}

void enable_interrupts()
{
    __asm__ volatile("sti" : : : "memory");
}

unsigned long long read_tsc()
{
    unsigned long long result;
    __asm__ volatile("rdtsc" : "=A"(result));
    return result;
}

unsigned long read_cr0()
{
    unsigned long result;
    __asm__ volatile("mov %%cr0, %0" : "=r"(result));
    return result;
}

void _get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
    asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

void _set_msr(uint32_t msr, uint32_t lo, uint32_t hi)
{
    asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}
