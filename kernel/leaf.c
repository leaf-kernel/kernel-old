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
#include <sys/time/rtc.h>

// Utility headers
#include <utils/check.h>
#include <utils/convertion.h>

// Parsing headers
#include <utils/parsing/elf.h>
#include <utils/parsing/ini.h>

int main() {
    _tty_flag_set(&currentTTY->ctx->cursor_enabled, true);
    update_memory();

    if(total_memory < 64000000) {
        plog_warn("Your computer only has %dMB of RAM. Leaf "
                  "recommends atleast 64MB!\033[0m",
                  bytes_to_mb(total_memory));
    } else {
        plog_ok("%d bytes OK", total_memory);
    }

    TestResult result = check_libc();
    if(result.failed == 0 && result.passed > 0) {
        plog_ok("All libc test passed.");
    } else {
        plog_warn("Only %d/%d libc tests passed.", result.passed,
                  result.passed + result.failed);
    }

    plog_ok("------- PCI ------");
    iterate_pci();
    plog_ok("------------------");

    char *hello;
    vfs_op_status status;

    VFS_t *vfs = (VFS_t *)__LEAF_GET_VFS__();

    status = drive_read(vfs, 0, "/sys/run/drivers/hello", &hello);

    if(status != STATUS_OK) {
        plog_fatal("Failed to read \"/sys/run/drivers/hello\"!");
        return LEAF_RETURN_FATAL;
    }

    plog_ok("Reached target \033[1mdrivers\033[0m");

    hlt();
    return LEAF_RETURN_SUCCESS;
}
