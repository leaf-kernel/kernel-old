#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <limine/limine.h>
#include <drivers/stdio/printf.h>
#include <arch/cpu/cpu.h>
#include <arch/pit/pit.h>
#include <arch/idt/idt.h>
#include <sys/logger.h>
#include "leaf.h"

void _start(void)
{
    init_idt();
    init_pit();
    hcf();
}