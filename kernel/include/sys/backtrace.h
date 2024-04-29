#ifndef __BACKTRACE_H__
#define __BACKTRACE_H__

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

struct stackframe
{
    struct stackframe *rbp;
    uint64_t rip;
} __attribute__((packed));

void backtrace();

#endif // __BACKTRACE_H__
