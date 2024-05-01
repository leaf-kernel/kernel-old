// Standard libgcc includes
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Arch imports
#include <arch/cpu/cpu.h>
#include <arch/cpu/utils.h>
#include <arch/pit/pit.h>
#include <arch/cpu/rsdt/rsdt.h>
#include <arch/x86_64/idt/idt.h>
#include <arch/x86_64/apic/apic.h>

// Memory imports
#include <libc/stdlib/memory/pmm.h>
#include <libc/stdlib/memory/kheap.h>

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
#include <sys/stable.h>
#include <sys/backtrace.h>
#include <sys/time/rtc.h>
#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

// Utility imports
#include <utils/convertion.h>
#include <utils/hash.h>
#include <utils/check.h>
#include <utils/john/john.h>

// Limine requests
#if defined(LEAF_LIMINE)
volatile struct limine_framebuffer_request framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};
volatile struct limine_module_request mod_request = {.id = LIMINE_MODULE_REQUEST, .revision = 0};
volatile struct limine_hhdm_request hhdm_request = {.id = LIMINE_HHDM_REQUEST, .revision = 0};
volatile struct limine_rsdp_request rsdp_request = {.id = LIMINE_RSDP_REQUEST, .revision = 0};
struct limine_framebuffer *framebuffer;
#endif

uint64_t hhdm_offset;
Ramdisk *initrd;
VFS_t *vfs;

// Utils
void *__LEAF_GET_INITRD__()
{
    return (void *)initrd;
}

void *__LEAF_GET_VFS__()
{
    return (void *)vfs;
}

// Kernel entry function
void _start(void)
{
#if defined(LEAF_LIMINE)
    hhdm_offset = hhdm_request.response->offset;
    framebuffer = framebuffer_request.response->framebuffers[0];
#endif

    dprintf("\033c");

    init_serial();
    init_idt();
    init_pit();
    init_pmm();
    init_apic();
    init_rtc();

    initrd = init_ramdisk((char *)(mod_request.response->modules[0]->address), mod_request.response->modules[0]->size);
    vfs = init_vfs();
    mount_drive(vfs, (uint64_t)initrd, TYPE_INITRD);
    init_stable();
    init_tty();
    tty_spawn(0, NULL, 1);
    tty_spawn(1, NULL, 2);
    tty_spawn(2, NULL, 3);
    tty_switch(0);

    cdlog("Kernel init done. On tty%04d", currentTTYid);

    int status = main();
    cdlog("Kernel exited with code %d", status);
    hlt();
}
