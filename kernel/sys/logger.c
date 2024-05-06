#include <sys/_config.h>
#include <sys/logger.h>
#include <sys/stable.h>

void debug_log(const char *file, const int line, const char *function,
               const char *fmt, ...) {
    if(_leaf_disable_pre_log)
        return;
    if(_leaf_log) {

        va_list args;
        va_start(args, fmt);
        dprintf("%s:%d (%s) - ", file, line, function);
        vdprintf(fmt, args);
        dprintf("\r\n");
        va_end(args);
    }
}

void cdebug_log(const char *function, const char *fmt, ...) {
    if(_leaf_disable_pre_log)
        return;
    if(_leaf_log) {
        va_list args;
        va_start(args, fmt);
        dprintf("[%-*.*s]: ", 14, 14, function);
        vdprintf(fmt, args);
        dprintf("\r\n");
        va_end(args);
    }
}
void pdebug_log(const char *file, const int line, const char *function,
                const char *fmt, ...) {
    if(_leaf_disable_pre_log)
        return;
    if(_leaf_log) {
        va_list args;
        va_start(args, fmt);
        printf("%s:%d (%s) - ", file, line, function);
        vprintf(fmt, args);
        printf("\r\n");
        va_end(args);
    }

    if(_leaf_should_log_serial_always) {
        va_list args;
        va_start(args, fmt);
        printf("%s:%d (%s) - ", file, line, function);
        vprintf(fmt, args);
        printf("\r\n");
        va_end(args);
    }
}
void pcdebug_log(const char *function, const char *fmt, ...) {
    if(_leaf_disable_pre_log)
        return;
    if(_leaf_log) {
        va_list args;
        va_start(args, fmt);
        printf("[%-*.*s]: ", 14, 14, function);
        vprintf(fmt, args);
        printf("\r\n");
        va_end(args);
    }

    if(_leaf_should_log_serial_always) {
        va_list args;
        va_start(args, fmt);
        dprintf("[%-*.*s]: ", 14, 14, function);
        vdprintf(fmt, args);
        dprintf("\r\n");
        va_end(args);
    }
}

void plog_ok(const char *fmt, ...) {
    if(_leaf_log && _leaf_disable_pre_log) {
        va_list args;
        va_start(args, fmt);
        printf("[  \033[32mOK\033[0m  ] ");
        vprintf(fmt, args);
        printf("\r\n");
        va_end(args);
    }
}

void plog_fail(const char *fmt, ...) {
    if(_leaf_log && _leaf_disable_pre_log) {
        va_list args;
        va_start(args, fmt);
        printf("[  \033[31mERROR\033[0m  ] ");
        vprintf(fmt, args);
        printf("\r\n");
        va_end(args);
    }
}

void plog_fatal(const char *fmt, ...) {
    if(_leaf_log && _leaf_disable_pre_log) {
        va_list args;
        va_start(args, fmt);
        printf("[  \033[91mFATAL\033[0m  ] ");
        vprintf(fmt, args);
        printf("\r\n");
        va_end(args);
    }
}

void plog_warn(const char *fmt, ...) {
    if(_leaf_log && _leaf_disable_pre_log) {
        va_list args;
        va_start(args, fmt);
        printf("[  \033[33mWARNING\033[0m  ] ");
        vprintf(fmt, args);
        printf("\r\n");
        va_end(args);
    }
}
