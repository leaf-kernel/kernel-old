#include <drivers/serial/serial.h>
#include <sys/time/rtc.h>

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

char bcd;

unsigned char read_register(unsigned char reg) {
    __asm__ volatile("cli");
    outb(RTC_COMMAND, reg);
    return inb(RTC_DATA);
    __asm__ volatile("sti");
}

void write_register(unsigned char reg, unsigned char value) {
    __asm__ volatile("cli");
    outb(RTC_COMMAND, reg);
    outb(RTC_DATA, value);
    __asm__ volatile("sti");
}

unsigned char bcd2bin(unsigned char in_bcd) {
    return (bcd) ? ((in_bcd >> 4) * 10) + (in_bcd & 0x0F) : in_bcd;
}

rtc_time_point rtc_get() {
    rtc_time_point target;

    target.seconds = bcd2bin(read_register(RTC_SECONDS));
    target.minutes = bcd2bin(read_register(RTC_MINUTES));
    target.hours = bcd2bin(read_register(RTC_HOURS));
    target.day_of_week = bcd2bin(read_register(RTC_DAY_OF_WEEK));
    target.day_of_month = bcd2bin(read_register(RTC_DAY));
    target.month = bcd2bin(read_register(RTC_MONTH));
    target.year = bcd2bin(read_register(RTC_YEAR));

    target.full[0] = target.seconds;
    target.full[1] = target.minutes;
    target.full[2] = target.hours;
    target.full[3] = target.day_of_month;
    target.full[4] = target.month;
    target.full[5] = target.year;

    return target;
}

uint64_t time_diff_seconds(rtc_time_point start, rtc_time_point end) {
    uint64_t start_seconds =
        start.seconds + start.minutes * 60 + start.hours * 3600;
    uint64_t end_seconds = end.seconds + end.minutes * 60 + end.hours * 3600;
    return end_seconds - start_seconds;
}

void init_rtc() {
    __asm__ volatile("cli");
    unsigned char status;
    status = read_register(RTC_STATUS);
    status |= 0x02;
    status |= 0x10;
    status &= ~0x20;
    status &= ~0x40;
    bcd = !(status & 0x04);
    write_register(RTC_STATUS, status);
    __asm__ volatile("sti");
    vvcplog("done.");
}

char *_get_month(int month) {
    char *a = NULL;

    switch(month) {
    case 1:
        a = "January";
        break;
    case 2:
        a = "February";
        break;
    case 3:
        a = "March";
        break;
    case 4:
        a = "April";
        break;
    case 5:
        a = "May";
        break;
    case 6:
        a = "June";
        break;
    case 7:
        a = "July";
        break;
    case 8:
        a = "August";
        break;
    case 9:
        a = "September";
        break;
    case 10:
        a = "October";
        break;
    case 11:
        a = "November";
        break;
    case 12:
        a = "December";
        break;
    }

    return a;
}

char *_get_day(int day) {
    char *a = NULL;

    switch(day) {
    case 1:
        a = "Sunday";
        break;
    case 2:
        a = "Monday";
        break;
    case 3:
        a = "Tuesday";
        break;
    case 4:
        a = "Wednesday";
        break;
    case 5:
        a = "Thursday";
        break;
    case 6:
        a = "Friday";
        break;
    case 7:
        a = "Saturday";
        break;
    }

    return a;
}