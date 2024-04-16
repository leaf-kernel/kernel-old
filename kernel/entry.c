// Standard libgcc includes
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Bootloader imports
#include <sys/limine.h>

// Arch specific imports
#include <arch/cpu/cpu.h>
#include <arch/pit/pit.h>
#include <arch/idt/idt.h>

// Memory imports
#include <memory/pmm.h>
#include <memory/kheap.h>

// Logging imports
#include <sys/logger.h>
#include <drivers/stdio/printf.h>
#include <drivers/tty/tty.h>

// Libc imports
#include <libc/math.h>
#include <libc/string.h>

// File imports
#include <fs/tar.h>
#include <fs/initrd.h>

// Sys import
#include <sys/limine.h>
#include <sys/leaf.h>

// Utility imports
#include <utils/convertion.h>


// Limine requests
volatile struct limine_framebuffer_request framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};
volatile struct limine_module_request mod_request = {.id = LIMINE_MODULE_REQUEST, .revision = 0};
volatile struct limine_hhdm_request hhdm_request = {.id = LIMINE_HHDM_REQUEST, .revision = 0};

// Global variables
struct limine_framebuffer *framebuffer;
uint64_t hhdm_offset;

// Kernel entry function
void _start(void)
{
    // init code
    hhdm_offset = hhdm_request.response->offset;
    framebuffer = framebuffer_request.response->framebuffers[0];

    // clear serial lolz
    dprintf("\033c");

    init_idt();
    init_pit();
    init_pmm();
    Ramdisk *initrd = init_ramdisk((char *)(mod_request.response->modules[0]->address), mod_request.response->modules[0]->size);
    cdebug_log(__func__, "Kernel init finished.");
    dprintf("\n");

    // Print out some system info
    dprintf("Leaf Version: %s\n", LEAF_VERSION);
    dprintf("Arch: %s\n", LEAF_ARCH);

    char brand[49];
    char vendor_string[13];
    get_intel_cpu_brand_string(brand);
    get_cpu_vendor_string(vendor_string);

    dprintf("CPU Vendor: %s\n", vendor_string);
    dprintf("CPU Brand: %s\n", brand);
    dprintf("Bootloader: %s\n", LEAF_BOOTLOADER);
    dprintf("Test file: %s\n", initrd->content[0]->file->content);
    hcf();
}