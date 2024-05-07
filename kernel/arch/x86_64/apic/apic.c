#include <arch/cpu/cpu.h>
#include <arch/x86_64/acpi/acpi.h>
#include <arch/x86_64/acpi/madt.h>
#include <arch/x86_64/apic/apic.h>
#include <arch/x86_64/cpu/cpuid.h>
#include <arch/x86_64/cpu/utils.h>
#include <arch/x86_64/pic/pic.h>

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

uint32_t rreg(uint16_t offset) {
    return *((uint32_t *)((uint64_t)0xfee00000 + offset));
}

void wreg(uint16_t offset, uint32_t val) {
    *((uint32_t *)((uint64_t)0xfee00000 + offset)) = val;
}

void init_apic() {

    if(!check_apic()) {
        vok("\033[1;31mError: APIC is not supported by your CPU!\033[0m");
        hcf();
        return;
    }

    if(!check_msr()) {
        vok("\033[1;31mError: MSR is not supported by your CPU!\033[0m");
        hcf();
        return;
    }

    init_acpi();
    enable_apic();

    vvok("%s: done.", __func__);
}

void enable_apic() {
    pic_disable();
    wreg(0xF0, rreg(0xF0) | 0x100);
    vok("enabled APIC");
}

void apic_eoi() {
    wreg(0xB0, 0);
    vok("Sent end-of-interupts!");
}
