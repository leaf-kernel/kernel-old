#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>
#include <sys/time/rtc.h>
#include <libc/stdlib/memory/kheap.h>
#include <arch/cpu/utils.h>

int main()
{
    rtc_time_point *time = (rtc_time_point *)kmalloc(sizeof(rtc_time_point));
    if (time == NULL)
    {
        dlog("Failed to allocate memory for the time!");
        return LEAF_RETURN_FATAL;
    }

    rtc_get(time);
    printf("%.3s %s %d %d:%d:%d 20%d\n", _get_day(time->day_of_week), _get_month(time->month), time->day_of_month, time->hours, time->minutes, time->seconds, time->year);
    hlt();
    return LEAF_RETURN_SUCCESS;
}