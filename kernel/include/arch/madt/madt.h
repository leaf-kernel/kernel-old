#ifndef __MADT_H__
#define __MADT_H__

#include <stdint.h>

#define MADT_SIGNATURE 0x41434950

typedef struct
{
    // APIC Table Header
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    uint64_t oem_table_id;
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;

    // MADT_specific fields
    uint32_t local_apic_addr;
    uint32_t flags;

} MADT;

typedef struct
{
    uint8_t type;
    uint8_t record_length;
} MADTEntryHeader;

typedef struct
{
    MADTEntryHeader header;
    uint8_t acpi_proccesor_id;
    uint8_t apic_id;
    uint32_t flags;
} MADTProcessorLocalAPIC;

typedef struct
{
    MADTEntryHeader header;
    uint8_t io_apic_id;
    uint8_t reserved;
    uint32_t io_apic_address;
    uint32_t globam_system_interrupt_base;
} MADTIOAPIC;

typedef struct
{
    MADTEntryHeader header;
    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t global_system_interrupt;
    uint16_t flags;
} MADTIOAPICInterruptSourceOverride;

typedef struct
{
    MADTEntryHeader header;
    uint8_t nmi_source;
    uint8_t reserved;
    uint16_t flags;
    uint32_t global_system_interrupt;
} MADTIOAPICNonmaskableInterruptSource;

typedef struct
{
    MADTEntryHeader header;
    uint8_t acpi_proccessor_id;
    uint16_t flags;
    uint8_t lint_nr;
} MADTLocalAPICNonmaskableInterrupts;

typedef struct
{
    MADTEntryHeader header;
    uint16_t reserved;
    uint64_t local_apic_physaddr;
} MADTLocalAPICAddressOverride;

typedef struct
{
    MADTEntryHeader header;
    uint16_t reserved;
    uint32_t processors_local_x2_apic_id;
    uint32_t flags;
    uint32_t acpi_id;
} MADTProcessorLocalx2APIC;

#endif // __MADT_H__