#include <include/sys/logger.h>

void debug_log(const char *file, const int line, const char *function, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    dprintf("%s:%d (%s) - ", file, line, function);
    vdprintf(fmt, args);
    dprintf("\n");
    va_end(args);
}

void cdebug_log(const char *function, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    dprintf("%s: ", function);
    vdprintf(fmt, args);
    dprintf("\n");
    va_end(args);
}