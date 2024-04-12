#ifndef __TAR_H__
#define __TAR_H__

#include <stdbool.h>
#include <stdint.h>
#include <libc/math.h>
#include <libc/string.h>
#include <fs/path.h>

#define RAMDISK_PATH_PREFIX "initrd"

typedef struct
{
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
} TARHeader;

typedef struct
{
    char *path;
    char *name;
    char *content;
    uint64_t size;
    bool directory;
} TARFile;

typedef struct
{
    TARFile *files;
    uint64_t fileCount;
} TAREntry;

void TARExtract(const char *raw, uint64_t size, TAREntry *tar);
void TARFree(TAREntry *tar);

#endif // __TAR_H__