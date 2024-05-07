#include <arch/x86_64/acpi/madt.h>

madt_t *madt_table;
uint32_t g_acpi_cpu_count;
apic_lapic_t *g_acpi_lapic[CONFIG_CPU_MAX];
uint8_t g_acpi_cpu_ids[CONFIG_CPU_MAX];

uint64_t g_ioapic_addr;
uint64_t g_lapic_addr;

// 16 IRQs
apic_iso_t *g_apic_isos[16];

void init_madt(madt_t *madt) {
    madt_table = madt;
    g_acpi_cpu_count = 0;
    g_lapic_addr = PHYS_TO_VIRT(madt->lapic_addr);
    vok("lapic addr: 0x%.8llx", g_lapic_addr);

    for(size_t i = 0; i < 16; i++) {
        g_apic_isos[i] = NULL;
    }

    uint8_t *ptr = (uint8_t *)(madt + 1);
    uint8_t *end = (uint8_t *)madt + madt->header.length;

    while(ptr < end) {
        apic_header_t *header = (apic_header_t *)ptr;

        switch(header->type) {
        case APIC_LAPIC: {
            apic_lapic_t *lapic = (apic_lapic_t *)ptr;
            vok("found lapic entry, id %i, flags 0x%x", g_acpi_cpu_count,
                lapic->flags);
            if(g_acpi_cpu_count < CONFIG_CPU_MAX) {
                g_acpi_lapic[g_acpi_cpu_count] = lapic;
                g_acpi_cpu_ids[g_acpi_cpu_count] = lapic->apic_id;
                ++g_acpi_cpu_count;
            }
            break;
        }
        case APIC_IOAPIC: {
            apic_ioapic_t *ioapic = (apic_ioapic_t *)ptr;
            g_ioapic_addr = PHYS_TO_VIRT(ioapic->ioapic_addr);
            vok("found ioapic %i, addr: 0x%.8llx, gsi_base: %i",
                ioapic->ioapic_id, ioapic->ioapic_addr, ioapic->gsi_base);
            break;
        }
        case APIC_ISO: {
            apic_iso_t *iso = (apic_iso_t *)ptr;
            g_apic_isos[iso->irq] = iso;
            vok("found ioapic iso, bus: %i, irq: %i, gsi: %i, flags: 0x%.4lx",
                iso->bus, iso->irq, iso->gsi, iso->flags);
            break;
        }
        case APIC_IOAPIC_NMI: {
            vok("found ioapic nmi");
            break;
        }
        case APIC_LAPIC_NMI: {
            vok("found lapic nmi");
            break;
        }
        case APIC_LAPIC_OVERRIDE: {
            vok("found lapic address override");
            break;
        }
        case APIC_X2APIC: {
            vok("found x2apic");
            break;
        }
        default: {
            vok("found invalid madt entry %i", header->type);
            break;
        }
        }

        ptr += header->length;
    }

    vvok("%s: done.", __func__);
}

uint32_t madt_get_iso(uint32_t irq) {
    if(g_apic_isos[irq] != NULL) {
        return g_apic_isos[irq]->gsi;
    }
    return irq;
}
