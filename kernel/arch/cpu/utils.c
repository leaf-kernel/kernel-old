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
