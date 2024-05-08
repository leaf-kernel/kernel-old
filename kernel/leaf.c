#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

// CPU related headers
#include <arch/cpu/cpu.h>
#include <arch/x86_64/cpu/cpuid.h>
#include <arch/x86_64/cpu/utils.h>

// x86_64 architecture specific headers
#include <arch/x86_64/acpi/mcfg.h>
#include <arch/x86_64/idt/idt.h>

// File system related headers
#include <fs/vfs.h>

// Memory management related headers
#include <libc/stdlib/memory/kheap.h>
#include <libc/stdlib/memory/vmm.h>

// System configuration headers
#include <sys/_config.h>

// System headers
#include <sys/backtrace.h>
#include <sys/run/runner.h>
#include <sys/time/rtc.h>

// Utility headers
#include <utils/check.h>
#include <utils/convertion.h>

// Parsing headers
#include <utils/parsing/elf.h>
#include <utils/parsing/ini.h>

int libc_test(service_t *self, void *in) {
    TestResult result = check_libc(self->config->verbose);
    if(result.failed == 0 && result.passed > 0) {
        ok("All libc tests passed.");
    } else {
        warn("Only %d/%d libc tests passed.", result.passed,
             result.passed + result.failed);
    }

    return LEAF_RETURN_SUCCESS;
}

int memory_check(service_t *self, void *in) {
    update_memory();

    if(total_memory < 64000000) {
        return SERVICE_WARN_MEMORY;
    } else {
        ok("%d bytes OK", total_memory);
    }

    return LEAF_RETURN_SUCCESS;
}

int map_kernel(service_t *self, void *data) {
    (void)data;
    (void)self;

    vvok("Kernel Start: 0x%lx", &__kernel_start);
    vvok(" - Text Start: 0x%lx", &__text_start);
    vvok(" - Text End: 0x%lx", &__text_end);
    vvok(" - ROData Start: 0x%lx", &__rodata_start);
    vvok(" - ROData End: 0x%lx", &__rodata_end);
    vvok(" - Data Start: 0x%lx", &__data_start);
    vvok(" - Data End: 0x%lx", &__data_end);
    vvok(" - BSS Start: 0x%lx", &__bss_start);
    vvok(" - BSS End: 0x%lx", &__bss_end);
    vvok("Kernel End: 0x%lx", &__kernel_end);

    vmm_map_range(&__kernel_start,
                  (void *)((uint64_t)&__text_start -
                           (uint64_t)kernel_addr_response->virtual_base +
                           (uint64_t)kernel_addr_response->physical_base),
                  &__kernel_end, _VMM_PRESENT);

    return LEAF_RETURN_SUCCESS;
}

int main(service_t *self, void *leaf_hdr) {
    __LEAF_HDR *hdr = (__LEAF_HDR *)leaf_hdr;
    if(hdr->magic != 0x76696570) {
        return SERVICE_ERROR_INVALID_MAGIC;
    }

    _tty_flag_set(&currentTTY->ctx->cursor_enabled, true);

    ok("-------- System Information --------");
    ok("Magic: 0x%X", hdr->magic);
    ok("Version: %d.%d.%d", hdr->version_major, hdr->version_minor,
       hdr->version_patch);
    ok("Build: %s", hdr->build);
    ok("Kernel: %s", hdr->kernel);
    ok("CPU Vendor: %s", hdr->cpu_vendor);
    ok("TTY: tty%03d", currentTTYid);
    ok("-------------------------------------");

    service_config_t memory_check_conf = {.name = "memory-check",
                                          .verbose = false,
                                          .run_once = true,
                                          .auto_start = true,
                                          .stop_when_done = true,
                                          .type = SERVICE_TYPE_CHECK,
                                          .runner = &memory_check};

    register_service(&memory_check_conf, NULL);

    service_config_t libc_test_conf = {.name = "libc-test",
                                       .verbose = false,
                                       .run_once = true,
                                       .auto_start = true,
                                       .stop_when_done = true,
                                       .type = SERVICE_TYPE_CHECK,
                                       .runner = &libc_test};

    register_service(&libc_test_conf, NULL);

    service_config_t driver_conf = {
        .name = "drivers",
        .verbose = false,
        .run_once = true,
        .auto_start = true,
        .stop_when_done = true,
        .type = SERVICE_TYPE_KINIT,
        .runner = &parse_elf_service,
    };

    // register_service(&driver_conf, "/sys/run/drivers/hello");

    service_config_t kernel_map = {
        .name = "kernel-mapping",
        .verbose = false,
        .run_once = true,
        .auto_start = true,
        .stop_when_done = true,
        .type = SERVICE_TYPE_KINIT,
        .runner = &map_kernel,
    };

    register_service(&kernel_map, NULL);

    ok("\033[1mpost-kinit\033[0m done.");

    hlt();
    return LEAF_RETURN_SUCCESS;
}
