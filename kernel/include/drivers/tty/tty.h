#ifndef __TTY_H__
#define __TTY_H__

#include <drivers/tty/nighterm/nighterm.h>
#include <memory/kheap.h>
#include <sys/logger.h>
#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>
#include <fs/vfs.h>

#include <stdint.h>
#include <stddef.h>

#define MAX_TTYS 50

extern uint8_t currentTTYid;

typedef struct
{
    struct nighterm_ctx *ctx;
    uint8_t id;
} TTY_t;

void init_tty();
void tty_spawn(uint8_t id, char *font);
void tty_switch(uint8_t id);
void tty_flush();
void tty_write(char ch);

#endif // __TTY_H__