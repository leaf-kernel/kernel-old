#include <arch/cpu/cpu.h>
#include <arch/x86_64/acpi/acpi.h>
#include <arch/x86_64/acpi/hpet.h>
#include <arch/x86_64/acpi/madt.h>
#include <arch/x86_64/apic/lapic.h>
#include <arch/x86_64/cpu/cpuid.h>
#include <arch/x86_64/cpu/utils.h>
#include <arch/x86_64/pic/pic.h>

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

uint32_t ticks = 0;

uint32_t rreg(uint16_t offset) {
    return *((uint32_t *)((uint64_t)madt_table->lapic_addr + offset));
}

void wreg(uint16_t offset, uint32_t val) {
    *((uint32_t *)((uint64_t)madt_table->lapic_addr + offset)) = val;
}

void apic_timer_stop() {
    wreg(LAPIC_TMRINITCNT, 0);
    wreg(LAPIC_LVT_TMR, LVT_MASKED);
}

void apic_timer_oneshot(uint64_t ms, uint8_t vec) {
    apic_timer_stop();
    wreg(LAPIC_TMRDIV, 0);
    wreg(LAPIC_LVT_TMR, vec);
    wreg(LAPIC_TMRINITCNT, ms * ticks);
}

void init_lapic() {

    if(!check_apic()) {
        fatal("APIC is not supported by your CPU!");
        hcf();
        return;
    }

    if(!check_msr()) {
        fatal("MSR is not supported by your CPU!");
        hcf();
        return;
    }

    pic_disable();

    wreg(LAPIC_SPURIOUS, 0x1FF);
    wreg(LAPIC_TASKPRIOR, 0);

    init_hpet();

    apic_timer_stop();
    wreg(LAPIC_TMRDIV, 0);
    wreg(LAPIC_LVT_TMR, 0);
    wreg(LAPIC_TMRINITCNT, 0xFFFFFFFF);

    hpet_usleep(100);

    ticks = 0xFFFFFFFF - rreg(LAPIC_TMRCURRCNT);
    apic_timer_stop();

    ok("LAPIC initialized!");
}

void apic_eoi() { wreg(LAPIC_apic_eoi, 0); }