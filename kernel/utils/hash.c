#include <libc/stdio/printf.h>
#include <libc/stdlib/atoi.h>
#include <libc/stdlib/memory/kheap.h>
#include <sys/leaf.h>
#include <utils/hash.h>

uint32_t hash_string(const char *in) {
    char *str = (char *)in;
    uint32_t hash = 5381;
    int c;

    while((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}