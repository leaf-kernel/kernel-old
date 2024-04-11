#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdarg.h>
#include <include/drivers/stdio/printf.h>

void debug_log(const char *function, const char *fmt, ...);

#endif // __LOGGER_H__