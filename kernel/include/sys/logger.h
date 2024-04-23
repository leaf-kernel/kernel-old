#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdarg.h>
#include <libc/stdio/printf.h>

void debug_log(const char *file, const int line, const char *function, const char *fmt, ...);
void cdebug_log(const char *function, const char *fmt, ...);

void pdebug_log(const char *file, const int line, const char *function, const char *fmt, ...);
void pcdebug_log(const char *function, const char *fmt, ...);

#endif // __LOGGER_H__