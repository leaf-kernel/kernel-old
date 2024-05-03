#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

#include <arch/cpu/cpu.h>
#include <arch/cpu/cpuid.h>
#include <arch/cpu/utils.h>
#include <fs/vfs.h>
#include <libc/stdlib/memory/kheap.h>
#include <sys/_config.h>
#include <sys/time/rtc.h>
#include <utils/parsing/ini.h>

int main() {
    __LEAF_DISABLE_LOG();
    tty_spawn(1, NULL, 2);
    __LEAF_ENABLE_LOG();
    cdlog("root @ tty%03d", currentTTYid);
    rtc_time_point time = rtc_get();

    cplog("%.3s %s %d %d:%d:%d 20%d", _get_day(time.day_of_week),
          _get_month(time.month), time.day_of_month, time.hours, time.minutes,
          time.seconds, time.year);

    hlt();
    return LEAF_RETURN_SUCCESS;
}