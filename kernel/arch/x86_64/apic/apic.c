#include <arch/x86_64/apic/apic.h>
#include <arch/cpu/cpu.h>
#include <arch/cpu/cpuid.h>
#include <arch/x86_64/acpi/acpi.h>
#include <arch/x86_64/acpi/madt.h>

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100
#define IA32_APIC_BASE_MSR_ENABLE 0x800

#define APIC_REGS_MSR_BASE 0x800

// TODO: Move these MSR util function to its own header
void _get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
    asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

void _set_msr(uint32_t msr, uint32_t lo, uint32_t hi)
{
    asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

void _set_apic_base(uintptr_t apic)
{
    uint32_t edx = 0;
    uint32_t eax = (apic & 0xfffff0000) | IA32_APIC_BASE_MSR_ENABLE;

#ifdef __PHYSICAL_MEMORY_EXTENSION__
    edx = (apic >> 32) & 0x0f;
#endif

    _set_msr(IA32_APIC_BASE_MSR, eax, edx);
}

uintptr_t _get_apic_base()
{
    uint32_t eax, edx;
    _get_msr(IA32_APIC_BASE_MSR, &eax, &edx);

#ifdef __PHYSICAL_MEMORY_EXTENSION__
    return (eax & 0xfffff000) | ((edx & 0x0f) << 32);
#else
    return (eax & 0xfffff000);
#endif
}

void init_apic()
{

    if (!check_apic())
    {
        cdlog("\033[1;31mError: APIC is not supported by your CPU!\033[0m");
        hcf();
        return;
    }

    if (!check_msr())
    {
        cdlog("\033[1;31mError: MSR is not supported by your CPU!\033[0m");
        hcf();
        return;
    }

    init_acpi();

    _set_apic_base(_get_apic_base());
    _write_reg(0xF0, _read_reg(0xF0) | 0x100);
    cdlog("r/w for registers isnt impelemented so no APIC :(");
    cdlog("todo.");
}
