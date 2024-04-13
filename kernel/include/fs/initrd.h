#ifndef __INITRD_H__
#define __INITRD_H__

#include <fs/tar.h>
#include <fs/path.h>

#include <stddef.h>

typedef struct
{
    TARFile *file;
    PathComponent *path;
} RamdiskEntry;

typedef struct RamdiskNode
{
    RamdiskEntry *entry;
    struct RamdiskNode *child;
    struct RamdiskNode *next;
} RamdiskNode;

typedef struct
{
    struct RamdiskNode *root;
} Ramdisk;

Ramdisk *init_ramdisk(const char *raw, const size_t size);

#endif // __INITRD_H__