// Standard libgcc includes
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Bootloader imports
#include <sys/limine.h>

// Arch imports
#include <arch/cpu/cpu.h>
#include <arch/pit/pit.h>
#include <arch/x86_64/idt/idt.h>

// Memory imports
#include <memory/pmm.h>
#include <memory/kheap.h>

// Logging imports
#include <sys/logger.h>
#include <drivers/tty/tty.h>

// Libc imports
#include <libc/math.h>
#include <libc/string.h>
#include <libc/stdio/printf.h>
#include <libc/stdlib/atoi.h>

// File imports
#include <fs/tar.h>
#include <fs/initrd.h>
#include <fs/vfs.h>

// Sys import
#include <sys/limine.h>
#include <sys/leaf.h>

// Utility imports
#include <utils/convertion.h>
#include <utils/hash.h>

// Limine requests
#if defined(LEAF_LIMINE)
volatile struct limine_framebuffer_request framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};
volatile struct limine_module_request mod_request = {.id = LIMINE_MODULE_REQUEST, .revision = 0};
volatile struct limine_hhdm_request hhdm_request = {.id = LIMINE_HHDM_REQUEST, .revision = 0};

struct limine_framebuffer *framebuffer;
#endif

uint64_t hhdm_offset;

// Kernel entry function
void _start(void)
{
#if defined(LEAF_LIMINE)
    hhdm_offset = hhdm_request.response->offset;
    framebuffer = framebuffer_request.response->framebuffers[0];
#endif

    // clear serial lolz
    dprintf("\033c");

    init_idt();
    init_pit();
    init_pmm();
    Ramdisk *initrd = init_ramdisk((char *)(mod_request.response->modules[0]->address), mod_request.response->modules[0]->size);
    VFS_t *vfs = init_vfs();
    (void)vfs;

    cdebug_log(__func__, "Kernel init finished.");
    dprintf("\r\n");
    dprintf("%s\r\n", initrd->content[find_file_by_hash(initrd, hash_string("/etc/motd"))]->file->content);

    // Print out some system info
    dprintf("Leaf Version: %s\r\n", LEAF_VERSION);
    dprintf("Leaf UUID: %s\r\n", LEAF_UUID);
    dprintf("Leaf Offset: 0x%08X\r\n", atoi(LEAF_OFFSET));
    dprintf("Arch: %s\r\n", LEAF_ARCH);

    char brand[49];
    char vendor_string[13];
    get_intel_cpu_brand_string(brand);
    get_cpu_vendor_string(vendor_string);

    dprintf("CPU Vendor: %s\r\n", vendor_string);
    dprintf("CPU Brand: %s\r\n", brand);
    dprintf("Bootloader: %s\r\n", LEAF_BOOTLOADER);
    hcf();
}
