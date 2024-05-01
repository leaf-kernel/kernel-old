#ifndef __RSDT_H__
#define __RSDT_H__

#include <libc/string.h>
#include <stdbool.h>
#include <stdint.h>

#define LEAF_INCLUDE_PRIVATE
#include <arch/cpu/cpu.h>
#include <sys/leaf.h>
#include <sys/limine.h>

#if !defined(LEAF_X64)
#error "RSDT Only supports x64!"
#endif

struct RSDPTable {
    struct ACPISDTHeader h;
    uint64_t sdt[];
} __attribute__((packed));

extern struct RSDPTable *rsdp_table;
void *find_sdt(char *signature);
void init_rsdt();

#endif  // __RSDT_H__
