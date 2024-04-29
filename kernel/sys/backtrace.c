#include <sys/backtrace.h>
#include <sys/stable.h>
#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

void backtrace()
{
    struct stackframe *frame;
    __asm__ volatile("mov %%rbp, %0" : "=g"(frame)::"memory");

    while (frame)
    {
        char *name = get_symbol_name((uint64_t)frame->rip);

        if (name != NULL)
            cdlog("[%s] <%.16llx>", name, frame->rip);
        else
            cdlog("[???] <%.16llx>", frame->rip);

        frame = frame->rbp;
    }
}
