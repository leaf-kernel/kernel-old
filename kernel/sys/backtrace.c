#include <sys/backtrace.h>
#include <sys/stable.h>
#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

void backtrace(int indent, void *rbp) {
    struct stackframe *frame = (struct stackframe *)rbp;

    while(frame) {
        if(frame->rip == 0)
            break;

        table_entry_t *symbol = lookup_symbol((uint64_t)frame->rip);

        if(symbol != NULL) {
            for(int i = 0; i < indent; i++)
                dprintf(" ");

            dprintf("[%.16lx]\t<%s+0x%04x>\r\n ", frame->rip, symbol->name,
                    frame->rip - symbol->addr);
        } else {
            for(int i = 0; i < indent; i++)
                dprintf(" ");

            dprintf("[%.16lx]\t<\?\?\?>\r\n", frame->rip);
        }
        frame = frame->rbp;
    }
}
