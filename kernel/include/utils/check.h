#ifndef __CHECK_H__
#define __CHECK_H__

#include <libc/string.h>
#include <libc/ctype.h>
#include <libc/stdlib/memory/kheap.h>
#include <libc/stdio/printf.h>

#include <stddef.h>

typedef struct
{
    int passed;
    int failed;
} TestResult;

TestResult check_libc();

#endif // __CHECK_H__
