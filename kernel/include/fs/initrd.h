#ifndef __INITRD_H__
#define __INITRD_H__

#include <stdbool.h>
#include <stdint.h>
#include <fs/tar.h>
#include <fs/path.h>
#include <libc/string.h>
#include <memory/kheap.h>

typedef struct DirectoryChild DirectoryChild;
typedef struct FileChild FileChild;

struct DirectoryChild
{
    char *name;
    DirectoryChild **dir_children;
    FileChild **file_children;
};

struct FileChild
{
    DirectoryChild *dir_parent;
    TARFile *entry;
};

typedef struct
{
    DirectoryChild *root;
    int fileCount;
} Ramdisk;

void init_ramdisk(Ramdisk *initrd, char *raw, size_t size);
void ramdisk_searchFile(Ramdisk *initrd, char *name);

#endif // __INITRD_H__