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

extern int st_entry_count;
extern table_entry_t st_entries[MAX_STABLE_COUNT];

void init_stable();
char *get_symbol_name(uint64_t addr);
int get_symbol_int(uint64_t addr);
uint64_t get_symbol_addr(char *name);
table_entry_t *lookup_symbol(uint64_t addr);

#endif // __STABLE_H__
