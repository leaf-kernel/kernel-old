#ifndef __BACKTRACE_H__
#define __BACKTRACE_H__

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

struct stackframe {
    struct stackframe *rbp;
    uint64_t rip;
} __attribute__((packed));

void backtrace(int indent, void *rbp);

#endif  // __BACKTRACE_H__
