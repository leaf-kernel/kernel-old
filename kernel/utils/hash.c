#include <utils/hash.h>
#include <libc/stdio/printf.h>
#include <sys/leaf.h>
#include <libc/stdlib/atoi.h>
#include <memory/kheap.h>

uint32_t hash_string(const char *in)
{   
    char* str = (char*)in;
    uint32_t hash = 0;
    uint32_t magic1 = 0x1b2c3d4e;
    uint32_t magic2 = 0x5f6a7b8c;
    uint32_t offset = (uint32_t)((atoi(LEAF_OFFSET) ^ magic1 / magic2));

    while (*str != '\0') {
        uint32_t i = *str;
        hash += i * offset;
        str++;
    }

    hash += magic1 << hash;
    hash += hash >> magic2;
    hash ^= offset;

    return hash;
}

