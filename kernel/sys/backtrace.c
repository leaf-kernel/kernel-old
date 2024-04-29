#include <sys/backtrace.h>
#include <sys/stable.h>
#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

void backtrace(int indent, void *rbp)
{
    struct stackframe *frame = (struct stackframe *)rbp;

    while (frame)
    {
        if (frame->rip == 0)
            break;

        table_entry_t *symbol = lookup_symbol((uint64_t)frame->rip);

        if (symbol != NULL)
        {
            for (int i = 0; i < indent; i++)
                dprintf(" ");

            dprintf("[%s] <%.16llx>\r\n", symbol->name, frame->rip);
        }
        else
        {
            for (int i = 0; i < indent; i++)
                dprintf(" ");

            dprintf("[???] <%.16llx>\r\n", frame->rip);
        }
        frame = frame->rbp;
    }
}
