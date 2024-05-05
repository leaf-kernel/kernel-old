#ifndef __TTY_H__
#define __TTY_H__

#include <drivers/tty/flanterm/backends/fb.h>
#include <drivers/tty/flanterm/flanterm.h>
#include <libc/stdlib/memory/kheap.h>
#include <sys/logger.h>
#define LEAF_INCLUDE_PRIVATE
#include <fs/vfs.h>
#include <sys/leaf.h>

#include <stddef.h>
#include <stdint.h>

#define MAX_TTYS 10

typedef struct {
    struct flanterm_context *ctx;
    uint8_t id;
    uint8_t mapped_com;
} TTY_t;

extern uint8_t currentTTYid;
extern TTY_t *currentTTY;

void init_tty();
void tty_destroy(uint8_t id);
void tty_spawn(uint8_t id, char *font, uint8_t mapped_com);
void tty_switch(uint8_t id);
void tty_flush();
void tty_write(char ch);

void _tty_flag_set(bool *flag, bool value);

#endif  // __TTY_H__