#include <utils/hash.h>
#include <sys/leaf.h>

uint32_t hash_string(const char *str)
{
    uint32_t hash = 5381;
    uint32_t prime = 31;
    uint32_t offset = (uint32_t)LEAF_OFFSET + 32;

    while (*str)
    {
        hash = (((hash ^ (*str++)) * prime ) - offset * hash) * offset;
    }

    hash = (hash ^ (hash >> 16)) * 0x85ebca6b;
    hash = (hash ^ (hash >> 13)) * 0xc2b2ae35;
    hash = hash ^ (hash >> 16);

    return hash;
}