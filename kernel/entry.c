// Standard libgcc includes
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Bootloader imports
#include <limine/limine.h>

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

// Libc imports
#include <libc/math.h>

// Leaf header import
#include <sys/leaf.h>

#if !defined(LEAF_LIMINE)
#error "Leaf currently only supports x86_64 with limine!"
#endif

// Limine requests
volatile struct limine_framebuffer_request framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};
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

    init_idt();
    init_pit();
    init_pmm();

    cdebug_log(__func__, "Kernel init finished.");
    dprintf("\n");
    dprintf("Leaf Version: %s\n", LEAF_VERSION);
    dprintf("Arch: %s\n", LEAF_ARCH);
    dprintf("CPU Model: %d\n", get_model());

    char brand[49];
    char vendor_string[13];
    get_intel_cpu_brand_string(brand);
    get_cpu_vendor_string(vendor_string);

    dprintf("CPU Vendor: %s\n", vendor_string);
    dprintf("CPU Brand: %s\n", brand);
    dprintf("APIC: %s\n", check_apic() ? "yes" : (check_apic() == false ? "no" : "?"));
    dprintf("Bootloader: %s\n", LEAF_BOOTLOADER);
    hcf();
}