#include <sys/logger.h>
#include <sys/stable.h>

void debug_log(const char *file, const int line, const char *function, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    dprintf("%s:%d (%s) - ", file, line, function);
    vdprintf(fmt, args);
    dprintf("\r\n");
    va_end(args);
}

void cdebug_log(const char *function, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    dprintf("%-*.*s: ", 14, 14, function);
    vdprintf(fmt, args);
    dprintf("\r\n");
    va_end(args);
}

void pdebug_log(const char *file, const int line, const char *function, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf("%s:%d (%s) - ", file, line, function);
    vprintf(fmt, args);
    printf("\r\n");
    va_end(args);
}
void pcdebug_log(const char *function, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf("%s: ", function);
    vprintf(fmt, args);
    printf("\r\n");
    va_end(args);
}
