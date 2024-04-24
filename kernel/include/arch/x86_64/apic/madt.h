#ifndef __MADT_H__
#define __MADT_H__

#include <stdint.h>
#include <stdbool.h>
#include <libc/string.h>

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>
#include <sys/limine.h>
#include <arch/cpu/cpu.h>
#include <arch/cpu/rsdt/rsdt.h>

struct MADTTable
{
    struct ACPISDTHeader h;
    uint32_t LocalAPICAddress;
    uint32_t Flags;
} __attribute__((packed));

extern struct MADTTable *madt_table;
void init_madt();

#endif // __MADT_H__
