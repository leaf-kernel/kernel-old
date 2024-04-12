#ifndef __PATH_H__
#define __PATH_H__

#include <memory/kheap.h>
#include <libc/string.h>

typedef struct
{
    char *name;
    int directory;
} PathComponent;

PathComponent *split_path(const char *path, int *num_components);
void free_components(PathComponent *components, int num_components);

#endif // __PATH_H__