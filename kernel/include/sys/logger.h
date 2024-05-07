#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <libc/stdio/printf.h>
#include <stdarg.h>
void plog_ok(const char *fmt, ...);

void ok(const char *fmt, ...);
void fail(const char *fmt, ...);
void fatal(const char *fmt, ...);
void warn(const char *fmt, ...);

#endif  // __LOGGER_H__