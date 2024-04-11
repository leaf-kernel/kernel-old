#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <limine/limine.h>
#include <drivers/stdio/printf.h>
#include <arch/cpu/cpu.h>
#include <arch/idt/idt.h>
#include <sys/logger.h>
#include "leaf.h"

void _start(void)
{
    dprintf("\033c");
    dprintf("Leaf Kernel, Arch: %s, Bootloader: %s, Compiled at %s with gcc v%s\n", LEAF_ARCH, LEAF_BOOTLOADER, __DATE__, __VERSION__);
    init_idt();
    hcf();
}