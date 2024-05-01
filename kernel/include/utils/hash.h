#ifndef __HASH_H__
#define __HASH_H__

#include <stddef.h>
#include <stdint.h>

#define HASH_STRING(str) hash_string(str)

uint32_t hash_string(const char *in);

#endif  // __HASH_H__