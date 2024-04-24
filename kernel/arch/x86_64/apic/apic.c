#include <arch/x86_64/apic/apic.h>
#include <arch/cpu/cpu.h>
#include <arch/cpu/cpuid.h>
#include <arch/x86_64/acpi/acpi.h>
#include <arch/x86_64/acpi/madt.h>

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

void init_apic()
{

    if (!check_apic())
    {
        cdlog("\033[1;31mError: APIC is not supported by your CPU!\033[0m");
        hcf();
        return;
    }

    init_acpi();

    cdlog("todo.");
}
