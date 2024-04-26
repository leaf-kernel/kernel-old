#ifndef __STACK_H__
#define __STACK_H__

#include <stdint.h>

struct stack_frame
{
    stack_frame *RBP;
    uint64_t RIP;
} __attribute__((packed));

#endif // __STACK_H__
