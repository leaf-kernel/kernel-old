#include <arch/x86_64/acpi/mcfg.h>
#include <arch/x86_64/acpi/rsdt.h>
#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

uint32_t num_entries;
device_config *entries;

void init_mcfg(mcfg_t *h) {
    if(h == NULL) {
        dlog("Failed to find the MCFG table!");
        hcf();
        return;
    }

    num_entries = (h->h.length - sizeof(mcfg_t)) / sizeof(device_config);
    entries = (device_config *)((uint8_t *)h + sizeof(mcfg_t));
    cdlog("done.");
}