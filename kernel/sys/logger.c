#include <sys/_config.h>
#include <sys/logger.h>
#include <sys/stable.h>

void ok(const char *fmt, ...) {
    // printf
    va_list args1;
    va_start(args1, fmt);
    printf("[  \033[32mOK\033[0m  ] ");
    vprintf(fmt, args1);
    printf("\r\n");
    va_end(args1);

    // // dprintf
    // va_list args2;
    // va_start(args2, fmt);
    // dprintf("[  \033[32mOK\033[0m  ] ");
    // vdprintf(fmt, args2);
    // dprintf("\r\n");
    // va_end(args2);
}

void fail(const char *fmt, ...) {
    // printf
    va_list args1;
    va_start(args1, fmt);
    printf("[  \033[31mERROR\033[0m  ] ");
    vprintf(fmt, args1);
    printf("\r\n");
    va_end(args1);

    // // dprintf
    // va_list args2;
    // va_start(args2, fmt);
    // dprintf("[  \033[31mERROR\033[0m  ] ");
    // vdprintf(fmt, args2);
    // dprintf("\r\n");
    // va_end(args2);
}

void warn(const char *fmt, ...) {
    // printf
    va_list args1;
    va_start(args1, fmt);
    printf("[  \033[33mWARNING\033[0m  ] ");
    vprintf(fmt, args1);
    printf("\r\n");
    va_end(args1);

    // // dprintf
    // va_list args2;
    // va_start(args2, fmt);
    // dprintf("[  \033[33mWARNING\033[0m  ] ");
    // vdprintf(fmt, args2);
    // dprintf("\r\n");
    // va_end(args2);
}

void fatal(const char *fmt, ...) {
    // printf
    va_list args1;
    va_start(args1, fmt);
    printf("[  \033[91mFATAL\033[0m  ] ");
    vprintf(fmt, args1);
    printf("\r\n");
    va_end(args1);

    // // dprintf
    // va_list args2;
    // va_start(args2, fmt);
    // dprintf("[  \033[91mFATAL\033[0m  ] ");
    // vdprintf(fmt, args2);
    // dprintf("\r\n");
    // va_end(args2);
}