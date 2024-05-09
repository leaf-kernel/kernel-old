#include <arch/x86_64/acpi/acpi.h>
#include <arch/x86_64/acpi/hpet.h>
#include <arch/x86_64/cpu/cpu.h>
#include <sys/logger.h>

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

hpet_regs *regs;

void init_hpet() {
    hpet_header *hpet = (hpet_header *)_find_sdt("HPET");
    if(hpet == NULL) {
        fatal("Failed to get the HPET table");
        hcf();
    }

    regs = (hpet_regs *)PHYS_TO_VIRT(hpet->addr);
    regs->counter_val = 0;
    regs->general_config = 1;

    vvok("%s: done", __func__);
}

void hpet_usleep(uint64_t us) {
    uint32_t clock_period = regs->capabilities >> 32;
    volatile size_t target_val =
        regs->counter_val + (us * (1000000000 / clock_period));
    while(regs->counter_val < target_val)
        ;
}