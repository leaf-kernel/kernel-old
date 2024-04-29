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
        cdlog("%016lx", frame->rip);
        char *name = get_symbol_name((uint64_t)frame->rip);

        if (name != NULL)
            cdlog("%s", name);

        frame = frame->rbp;
    }
}
