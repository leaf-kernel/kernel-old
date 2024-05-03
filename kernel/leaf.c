#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

#include <arch/cpu/cpu.h>
#include <arch/cpu/cpuid.h>
#include <arch/cpu/utils.h>
#include <drivers/pci/pci.h>
#include <fs/vfs.h>
#include <libc/stdlib/memory/kheap.h>
#include <sys/_config.h>
#include <sys/time/rtc.h>
#include <utils/parsing/ini.h>

int main() {
    printf("root @ tty%03d\r\n", currentTTYid);
    rtc_time_point time = rtc_get();

    printf("%.3s %s %d %d:%d:%d 20%d\r\n", _get_day(time.day_of_week),
           _get_month(time.month), time.day_of_month, time.hours, time.minutes,
           time.seconds, time.year);

    init_pci();

    hlt();
    return LEAF_RETURN_SUCCESS;
}