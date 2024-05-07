#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

// CPU related headers
#include <arch/cpu/cpu.h>
#include <arch/cpu/cpuid.h>
#include <arch/cpu/utils.h>

// x86_64 architecture specific headers
#include <arch/x86_64/acpi/mcfg.h>
#include <arch/x86_64/idt/idt.h>

// File system related headers
#include <fs/vfs.h>

// Memory management related headers
#include <libc/stdlib/memory/kheap.h>

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

int main(service_t *self, void *leaf_hdr) {
    __LEAF_HDR *hdr = (__LEAF_HDR *)leaf_hdr;
    if(hdr->magic != 0x7F61F3C0) {
        return SERVICE_ERROR_INVALID_MAGIC;
    }

    _tty_flag_set(&currentTTY->ctx->cursor_enabled, true);

    ok("Magic: 0x%X", hdr->magic);
    ok("Version: %d.%d.%d", hdr->version_major, hdr->version_minor,
       hdr->version_patch);
    ok("Build: %s", hdr->build);
    ok("Kernel: %s", hdr->kernel);
    ok("CPU Vendor: %s", hdr->cpu_vendor);
    ok("TTY: tty%03d", currentTTYid);

    service_config_t memory_check_conf = {.name = "memory-check",
                                          .verbose = false,
                                          .run_once = true,
                                          .auto_start = true,
                                          .stop_when_done = true,
                                          .type = SERVICE_TYPE_KERNEL,
                                          .runner = &memory_check};

    register_service(&memory_check_conf, NULL);

    service_config_t libc_test_conf = {.name = "libc-test",
                                       .verbose = false,
                                       .run_once = true,
                                       .auto_start = true,
                                       .stop_when_done = true,
                                       .type = SERVICE_TYPE_KERNEL,
                                       .runner = &libc_test};

    register_service(&libc_test_conf, NULL);

    service_config_t driver_conf = {
        .name = "drivers",
        .verbose = false,
        .run_once = true,
        .auto_start = true,
        .stop_when_done = true,
        .type = SERVICE_TYPE_KERNEL,
        .runner = &parse_elf_service,
    };

    register_service(&driver_conf, "/sys/run/drivers/hello");

    _int(3);

    hlt();
    return LEAF_RETURN_SUCCESS;
}
