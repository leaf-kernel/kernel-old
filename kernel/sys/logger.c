#include <include/sys/logger.h>

void debug_log(const char *function, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    dprintf("%s: ", function);
    vdprintf(fmt, args);
    va_end(args);
}
