#include <arch/x86_64/apic/apic.h>
#include <arch/cpu/cpu.h>
#include <arch/cpu/cpuid.h>

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

void init_apic()
{

    if (check_apic())
    {
        cdlog("\033[1;32mAPIC is supported by your CPU\033[0m");
    }
    else
    {
        cdlog("\033[1;32mError: APIC is not supported by your CPU!\033[0m");
        hcf();
        return;
    }
}