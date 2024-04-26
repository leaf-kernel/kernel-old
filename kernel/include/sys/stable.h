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
} table_entry_t;

void init_stable();

#endif // __STABLE_H__
