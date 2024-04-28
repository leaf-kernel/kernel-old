#include <sys/backtrace.h>
#include <sys/stable.h>
#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

void backtrace()
{
    uint64_t *base;
    __asm__ volatile("mov %%rbp, %0" : "=g"(base)::"memory");
    struct stackframe *frame = (struct stackframe *)*base;

    while (1)
    {
        if (frame->rip == (uint64_t)NULL || base == NULL || frame->rip < (uint64_t)0xffffffff80000000)
        {
            break;
        }

        int idx = -1;
        for (int i = 0; st_entries[i].addr < UINTPTR_MAX; i++)
        {
            if (st_entries[i].addr < frame->rip && st_entries[i + 1].addr >= frame->rip)
            {
                idx = i;
            }
        }

        if (idx < 0)
        {
            cdlog("    [%.16lx]  <\?\?\?>", frame->rip);
        }
        else
        {
            cdlog("    [%.16lx]  <%s+0x%04x>", frame->rip, st_entries[idx].name, frame->rip - st_entries[idx].addr);
        }

        frame = frame->rbp;
    }

    cdlog("End of trace.");
}