#ifndef __INITRD_H__
#define __INITRD_H__

#include <fs/tar.h>
#include <fs/path.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    TARFile *file;
    uint32_t hash;
    PathComponent *path;
} RamdiskEntry;

typedef struct
{
    RamdiskEntry **content;
    int count;
} Ramdisk;

Ramdisk *init_ramdisk(const char *raw, const size_t size);
int find_file_by_hash(Ramdisk *initrd, uint32_t hash);

#endif // __INITRD_H__