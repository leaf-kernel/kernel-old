#ifndef __KHEAP_H__
#define __KHEAP_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <libc/string.h>

void *kmalloc(size_t size);
void *kcalloc(size_t size);
void *krealloc(void *old, size_t size);
void kfree(void *tofree);

#endif // __KHEAP_H__
