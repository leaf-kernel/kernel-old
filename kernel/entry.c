// Standard libgcc includes
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Arch imports
#include <arch/cpu/cpu.h>
#include <arch/cpu/rsdt/rsdt.h>
#include <arch/cpu/utils.h>
#include <arch/pit/pit.h>
#include <arch/x86_64/apic/apic.h>
#include <arch/x86_64/idt/idt.h>

// Memory imports
#include <libc/stdlib/memory/kheap.h>
#include <libc/stdlib/memory/pmm.h>
#include <libc/stdlib/memory/vmm.h>

// Logging imports
#include <drivers/tty/tty.h>
#include <sys/logger.h>

// Libc imports
#include <libc/math.h>
#include <libc/stdio/printf.h>
#include <libc/stdlib/atoi.h>
#include <libc/string.h>

// File imports
#include <fs/initrd.h>
#include <fs/tar.h>
#include <fs/vfs.h>

// Sys import
#include <sys/backtrace.h>
#include <sys/limine.h>
#include <sys/run/runner.h>
#include <sys/stable.h>
#include <sys/time/rtc.h>
#define LEAF_INCLUDE_PRIVATE
#include <sys/_config.h>
#include <sys/leaf.h>

// Utility imports
#include <utils/check.h>
#include <utils/convertion.h>
#include <utils/hash.h>
#include <utils/john/john.h>

// Limine requests
#if defined(LEAF_LIMINE)
volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};
volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};
volatile struct limine_hhdm_request hhdm_request = {.id = LIMINE_HHDM_REQUEST,
                                                    .revision = 0};
volatile struct limine_rsdp_request rsdp_request = {.id = LIMINE_RSDP_REQUEST,
                                                    .revision = 0};
struct limine_framebuffer *framebuffer;
#endif

uint64_t hhdm_offset;
Ramdisk *initrd;
VFS_t *vfs;
bool _leaf_log;
bool _leaf_should_clear_serial;
bool _leaf_should_flush_serial;
bool _leaf_should_flush_tty;
bool _leaf_disable_pre_log;
bool _leaf_should_log_serial_always;

// Utils
void *__LEAF_GET_INITRD__() { return (void *)initrd; }

void *__LEAF_GET_VFS__() { return (void *)vfs; }
int kinit(service_t *self, void *args);

// Kernel entry function
void _start(void) {
    service_config_t kinit_conf = {
        .name = "kinit",
        .verbose = true,
        .run_once = true,
        .auto_start = true,
        .stop_when_done = true,
        .runner = &kinit,
    };
    register_service(&kinit_conf, NULL);

    hcf();
}

int kinit(service_t *self, void *args) {

#if defined(LEAF_LIMINE)
    hhdm_offset = hhdm_request.response->offset;
    framebuffer = framebuffer_request.response->framebuffers[0];
#endif
    __LEAF_ENABLE_LOG();
    __LEAF_FLUSH_TTY();
    __LEAF_CLEAR_SERIAL();
    __LEAF_FLUSH_SERIAL();
    __LEAF_ENABLE_PRE_LOG();

    __LEAF_LOG_SERIAL_ALWAYS();
    init_serial();
    flush_serial();
    init_rtc();
    init_idt();
    init_pit();
    init_pmm();
    __LEAF_DONT_LOG_SERIAL_ALWAYS();
    __LEAF_DONT_CLEAR_SERIAL();
    __LEAF_DONT_FLUSH_SERIAL();
    init_tty();

#ifdef __LEAF_MAP_TTY__
    tty_spawn(0, NULL, 1);
#else
    tty_spawn(0, NULL, 0);
#endif

    init_vmm();
    init_apic();

    initrd = init_ramdisk((char *)(mod_request.response->modules[0]->address),
                          mod_request.response->modules[0]->size);
    vfs = init_vfs();
    mount_drive(vfs, (uint64_t)initrd, TYPE_INITRD);
    init_stable();

    __LEAF_DISABLE_PRE_LOG();

    service_config_t post_kinit_conf = {
        .name = "post-kinit",
        .verbose = true,
        .run_once = true,
        .auto_start = true,
        .stop_when_done = true,
        .runner = &main,
    };

    __LEAF_HDR post_kinit_hdr = {
        .magic = 0x7F61F3C0,
        .version_major = 0,
        .version_minor = 3,
        .version_patch = 5,
        .build = "Development",
    };

    register_service(&post_kinit_conf, &post_kinit_hdr);
    return 0;
}