#ifndef __MADT_H__
#define __MADT_H__

#include <stdint.h>

#define MADT_SIG 0x41434950

typedef struct
{
    char sig[4];
    uint32_t len;
    uint8_t rev;
    uint8_t chksum;
    char oem_id[6];
    uint64_t oem_table_id;
    uint32_t oem_rev;
    uint32_t creator_id;
    uint32_t creator_rev;
    uint32_t lapic_addr;
    uint32_t flags;
} MADT;

typedef struct
{
    uint8_t type;
    uint8_t len;
} MADT_Entry;

typedef struct
{
    MADT_Entry header;
    uint8_t acpi_id;
    uint8_t apic_id;
    uint32_t flags;
} MADT_CPU_LAPIC;

typedef struct
{
    MADT_Entry header;
    uint8_t ioapic_id;
    uint8_t reserved;
    uint32_t ioapic_addr;
    uint32_t gsi_base;
} MADT_IOAPIC;

typedef struct
{
    MADT_Entry header;
    uint8_t bus_src;
    uint8_t irq_src;
    uint32_t gsi;
    uint16_t flags;
} MADT_IOAPIC_IntSrcOverride;

typedef struct
{
    MADT_Entry header;
    uint8_t nmi_src;
    uint8_t reserved;
    uint16_t flags;
    uint32_t gsi;
} MADT_IOAPIC_NMI;

typedef struct
{
    MADT_Entry header;
    uint8_t acpi_id;
    uint16_t flags;
    uint8_t lint_nr;
} MADT_LAPIC_NMI;

typedef struct
{
    MADT_Entry header;
    uint16_t reserved;
    uint64_t lapic_phys_addr;
} MADT_LAPIC_Address;

typedef struct
{
    MADT_Entry header;
    uint16_t reserved;
    uint32_t proc_local_x2_apic_id;
    uint32_t flags;
    uint32_t acpi_id;
} MADT_CPU_LAPIC_x2;

#endif // __MADT_H__
