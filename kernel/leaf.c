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

// Backtrace related headers
#include <sys/backtrace.h>

// Real-time clock related headers
#include <sys/time/rtc.h>

// Utility headers
#include <utils/convertion.h>

int main() {
    _tty_flag_set(&currentTTY->ctx->cursor_enabled, true);
    update_memory();

    if(total_memory < 64000000)
        plog_warn("Your computer only has %dMB of RAM. Leaf "
                  "recommends atleast 64MB!\033[0m",
                  bytes_to_mb(total_memory));
    else
        plog_ok("%d bytes OK", total_memory);
    rtc_time_point time = rtc_get();

    plog_ok("%.3s %.3s %.2d %.2d:%.2d:%.2d @ tty%03d",
            _get_day(time.day_of_week), _get_month(time.month),
            time.day_of_month, time.hours, time.minutes, time.seconds,
            currentTTYid);

    hlt();
    return LEAF_RETURN_SUCCESS;
}
