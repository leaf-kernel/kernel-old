#ifndef __STABLE_H__
#define __STABLE_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <fs/vfs.h>

#define MAX_STABLE_COUNT 65535

typedef struct
{
    uint64_t addr;
    char *name;
    char id;
} table_entry_t;

void init_stable();
table_entry_t get_symbol(uint64_t addr);

#endif // __STABLE_H__
