#ifndef __RSDT_H_
#define __RSDT_H_

#include <arch/x86_64/acpi/acpi.h>
#include <stdint.h>

typedef struct {
    sdt_t header;
    uint32_t sdt[];
} __attribute__((packed)) rsdt_t;

typedef struct {
    sdt_t header;
    uint64_t sdt[];
} __attribute__((packed)) xsdt_t;

extern xsdt_t *g_xsdt;
extern rsdt_t *g_rsdt;

void init_rsdt();

#endif  // __RSDT_H_
