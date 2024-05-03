#ifndef __MCFG_H__
#define __MCFG_H__

#include <arch/cpu/cpu.h>
#include <arch/x86_64/acpi/rsdt.h>
#include <stdint.h>

typedef struct {
    sdt_t h;
    uint64_t reserved;
} __attribute__((packed)) mcfg_t;

typedef struct {
    uint64_t base_address;
    uint16_t PCI_seg_group;
    uint8_t start_bus;
    uint8_t end_bus;
    uint32_t reserved;
} __attribute__((packed)) device_config;

void init_mcfg(mcfg_t *h);

#endif  // __MCFG_H__