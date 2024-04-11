#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <limine/limine.h>
#include <include/drivers/stdio/printf.h>
#include <include/drivers/cpu/cpu.h>
#include <include/sys/logger.h>
#include <leaf.h>

void _start(void) {
    dprintf("\033c");
    dprintf("Leaf Kernel, Arch: %s, Bootloader: %s, Compiled at %s with gcc v%s\n\n", LEAF_ARCH, LEAF_BOOTLOADER, __DATE__, __VERSION__);
    hcf();
}