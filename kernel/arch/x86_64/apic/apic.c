#include <arch/cpu/cpu.h>
#include <arch/cpu/cpuid.h>
#include <arch/cpu/utils.h>
#include <arch/x86_64/acpi/acpi.h>
#include <arch/x86_64/acpi/madt.h>
#include <arch/x86_64/apic/apic.h>
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
        cdlog("\033[1;31mError: APIC is not supported by your CPU!\033[0m");
        hcf();
        return;
    }

    if(!check_msr()) {
        cdlog("\033[1;31mError: MSR is not supported by your CPU!\033[0m");
        hcf();
        return;
    }

    init_acpi();
    enable_apic();

    cdlog("done.");
}

void enable_apic() {
    pic_disable();
    wreg(0xF0, rreg(0xF0) | 0x100);
    cdlog("enabled APIC");
}

void apic_eoi() {
    wreg(0xB0, 0);
    vcdlog("Sent end-of-interupts!");
}
