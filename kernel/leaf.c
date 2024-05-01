#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

#include <sys/time/rtc.h>
#include <libc/stdlib/memory/kheap.h>

int main()
{
    rtc_time_point *time = (rtc_time_point *)kmalloc(sizeof(rtc_time_point));
    if (time == NULL)
    {
        dlog("Failed to init time!");
        hcf();
    }

    rtc_get(time);

    cplog("%.3s %s %d %d:%d:%d 20%d", _get_day(time->day_of_week), _get_month(time->month), time->day_of_month, time->hours, time->minutes, time->seconds, time->year);

    return 0;
}