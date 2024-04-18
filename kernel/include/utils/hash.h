#ifndef __HASH_H__
#define __HASH_H__

#include <stdint.h>
#include <stddef.h>

#define HASH_STRING(str) hash_string(str)

uint32_t hash_string(const char *in);

#endif // __HASH_H__