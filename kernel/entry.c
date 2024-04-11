#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine/limine.h>
#include <drivers/stdio/printf.h>
#include <arch/cpu/cpu.h>
#include <memory/pmm.h>
#include <memory/kheap.h>
#include <arch/pit/pit.h>
#include <arch/idt/idt.h>
#include <sys/logger.h>
#include "leaf.h"

#if !defined(LEAF_LIMINE)
#error "Leaf currently only supports x86_64 with limine!"
#endif

// Limine requests
volatile struct limine_framebuffer_request framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};
volatile struct limine_hhdm_request hhdm_request = {.id = LIMINE_HHDM_REQUEST, .revision = 0};

// Global variables
struct limine_framebuffer *framebuffer;
uint64_t hhdm_offset;

void _start(void)
{
    // init code
    hhdm_offset = hhdm_request.response->offset;
    framebuffer = framebuffer_request.response->framebuffers[0];

    init_idt();
    init_pit();
    init_pmm();

    dprintf("Leaf Kernel, Arch: %s, Bootloader: %s, GCC Version: %s, Compile Date: %s\n", LEAF_ARCH, LEAF_BOOTLOADER, __VERSION__, __DATE__);

    hcf();
}