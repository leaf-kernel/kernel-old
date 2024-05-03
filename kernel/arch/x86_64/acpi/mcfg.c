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
    cdlog("Entries: %d", num_entries);
    for(int i = 0; i < num_entries; i++)
        vcdlog("Base Address: 0x%.16llx", entries[i].base_address);
    cdlog("done.");
}

void iterate_pci() {
    for(int bus = 0; bus < 8; bus++) {
        for(int device = 0; device < 32; device++) {
            int numfuncs = pci_read(bus, device, 0, 0x0E) & (1 << 7) ? 8 : 1;
            for(int function = 0; function < numfuncs; function++) {
                uint16_t vendor_id = pci_read(bus, device, function, 0x00);
                uint16_t device_id = pci_read(bus, device, function, 0x02);
                uint16_t class_id = pci_read(bus, device, function, 0x0b);
                uint16_t subclass_id = pci_read(bus, device, function, 0x0a);
                uint16_t interface_id = pci_read(bus, device, function, 0x09);
                uint16_t revision = pci_read(bus, device, function, 0x08);
                uint16_t interrupt = pci_read(bus, device, function, 0x3c);

                if(vendor_id == 0x0000 || vendor_id == 0xFFFF) {
                    continue;
                }
                cplog(
                    "PCI Bus: %02d Device: %02d ID: %04X Function: %d USB: %s",
                    (uint8_t)(bus & 0xFF), (uint8_t)(device & 0xFF), device_id,
                    ((uint8_t)((function & 0xFF))),
                    ((class_id == 0x0C) && (subclass_id == 0x03)) ? "yes"
                                                                  : "no");
            }
        }
    }
}

uint32_t *pci_getaddr(uint8_t bus, uint8_t dev, uint8_t func, uint8_t off) {
    for(size_t i = 0; i < num_entries; i++) {
        if(bus >= entries[i].start_bus && bus <= entries[i].end_bus) {
            uint64_t addr = (entries[i].base_address + (bus << 20) +
                             (dev << 15) + (func << 12) + off);
            return (uint32_t *)addr;
        }
    }
    return 0;
}

uint32_t pci_read(uint8_t bus, uint8_t dev, uint8_t func, uint8_t off) {

    return pci_getaddr(bus, dev, func, off)[0];
}
void pci_write(uint8_t bus, uint8_t dev, uint8_t func, uint8_t off,
               uint32_t val) {
    pci_getaddr(bus, dev, func, off)[0] = val;
}