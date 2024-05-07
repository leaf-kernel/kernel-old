#include <arch/x86_64/cpu/cpuid.h>
#include <libc/stdlib/memory/kheap.h>
#include <stddef.h>

int cpuid_string(int code, uint32_t where[4]) {
    asm volatile("cpuid"
                 : "=a"(*where), "=b"(*(where + 1)), "=c"(*(where + 2)),
                   "=d"(*(where + 3))
                 : "a"(code));
    return (int)where[0];
}