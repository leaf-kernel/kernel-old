#define LEAF_INCLUDE_PRIVATE
#include <arch/cpu/utils.h>
#include <fs/vfs.h>
#include <libc/stdlib/memory/kheap.h>
#include <sys/leaf.h>
#include <sys/time/rtc.h>
#include <utils/parsing/ini.h>

int main() {
    rtc_time_point *time = (rtc_time_point *)kmalloc(sizeof(rtc_time_point));
    if(time == NULL) {
        dlog("Failed to allocate memory for the time!");
        return LEAF_RETURN_FATAL;
    }

    rtc_get(time);
    cplog("%.3s %s %d %d:%d:%d 20%d", _get_day(time->day_of_week),
          _get_month(time->month), time->day_of_month, time->hours,
          time->minutes, time->seconds, time->year);

    vfs_op_status status;
    char *buff = (char *)kmalloc(sizeof(char));
    if(buff == NULL)
        return LEAF_RETURN_FATAL;

    status = drive_read(__LEAF_GET_VFS__(), 0, "/sys/kernel/.config", &buff);

    if(status != STATUS_OK)
        return LEAF_RETURN_FATAL;

    struct parsed_ini ini = parse_ini(buff);
    cdlog("Release Type: %s", ini.data[0].value);
    hlt();
    return LEAF_RETURN_SUCCESS;
}