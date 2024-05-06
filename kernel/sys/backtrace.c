#include <sys/backtrace.h>
#include <sys/stable.h>
#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>
#include <sys/time/rtc.h>

void backtrace(void *rbp, uint64_t caller) {
    struct stackframe *frame = (struct stackframe *)rbp;

    rtc_time_point time = rtc_get();

    printf("┌───────────────────────────────────────────────────┐\r\n");
    printf("│ Backtrace at %.3s %.3s %.2d %.2d:%.2d:%.2d @ tty%03d         "
           "│\r\n",
           _get_day(time.day_of_week), _get_month(time.month),
           time.day_of_month, time.hours, time.minutes, time.seconds,
           currentTTYid);
    printf("├───────────────────────────────────────────────────┤\r\n");

    table_entry_t *func = lookup_symbol(caller);
    if(func != NULL) {
        char func_name[30];
        snprintf(func_name, sizeof(func_name), "%s+0x%04x", func->name,
                 caller - func->addr);
        int func_name_length = strlen(func_name);
        if(func_name_length > 25)
            func_name[22] = func_name[23] = func_name[24] = '.';
        printf("│ [%.16lx]\t<%-25s> │\r\n", caller, func_name);
    } else {
        printf("│ [%.16lx]\t<\?\?\?>   │\r\n", caller);
    }

    while(frame) {
        if(frame->rip == 0)
            break;

        table_entry_t *symbol = lookup_symbol((uint64_t)frame->rip);

        if(symbol != NULL) {
            char symbol_name[30];
            snprintf(symbol_name, sizeof(symbol_name), "%s+0x%04x",
                     symbol->name, frame->rip - symbol->addr);
            int symbol_name_length = strlen(symbol_name);
            if(symbol_name_length > 25)
                symbol_name[22] = symbol_name[23] = symbol_name[24] = '.';
            printf("│ ↕ [%.16lx]\t<%-25s> │\r\n", frame->rip, symbol_name);
        } else {
            printf("│ ↕ [%.16lx]\t<\?\?\?>   │\r\n", frame->rip);
        }
        frame = frame->rbp;
    }

    printf("└───────────────────────────────────────────────────┘\r\n");
}
