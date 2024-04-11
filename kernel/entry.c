#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <limine/limine.h>
#include <include/drivers/stdio/printf.h>
#include <include/drivers/cpu/cpu.h>
#include <include/sys/logger.h>


void _start(void) {
    dprintf("\033c");
    debug_log(__func__, "Hello, World!");
    hcf();
}