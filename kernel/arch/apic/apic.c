#include <arch/apic/apic.h>
#include <arch/cpu/cpu.h>
#include <sys/logger.h>

bool check_apic()
{
    bool apic_supported = false;
    unsigned int edx;

    __asm__ volatile(
        "mov $1, %%eax \n\t"
        "cpuid \n\t"
        "mov %%edx, %0 \n\t"
        : "=r"(edx)
        :
        : "%eax", "%ebx", "%ecx");

    if (edx & (1 << 9))
    {
        apic_supported = true;
    }

    return apic_supported;
}

void init_apic()
{
    if (!check_apic())
    {
        cdebug_log(__func__, "APIC is not supported by this CPU!");
        hcf();
    }

    if (!cpuHasMSR())
    {
        cdebug_log(__func__, "CPU Doesnt have MSR (Model Specific Registers)!");
        hcf();
    }

    cdebug_log(__func__, "APIC Support is currently not working.");
}