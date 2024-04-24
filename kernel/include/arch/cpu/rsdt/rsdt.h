#ifndef __RSDT_H__
#define __RSDT_H__

#include <stdint.h>
#include <stdbool.h>
#include <libc/string.h>

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>
#include <sys/limine.h>
#include <arch/cpu/cpu.h>

#if !defined(LEAF_X64)
#error "RSDT Only supports x64!"
#endif

struct RSDPTable
{
    struct ACPISDTHeader h;
    uint64_t sdt[];
} __attribute__((packed));

extern struct RSDPTable *rsdp_table;

void init_rsdt();

#endif // __RSDT_H__
