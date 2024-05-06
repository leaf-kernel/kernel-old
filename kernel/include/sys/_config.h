#ifndef __CONFIG_H__
#define __CONFIG_H__

#define __LEAF_VERBOSE__
#define __LEAF_VVERBOSE__
// #define __LEAF_MAP_TTY__

// Extra-Extra verbose logging is only for the symbol table atm.
// #define __LEAF_VVVERBOSE__
#define __LEAF_DEBUG_WRAPPERS__
#define __LEAF_DEBUG__

#define CONFIG_CPU_MAX 256

#include <stdbool.h>

extern bool _leaf_log;
extern bool _leaf_should_clear_serial;
extern bool _leaf_should_flush_serial;
extern bool _leaf_should_flush_tty;
extern bool _leaf_disable_pre_log;

#define __LEAF_DISABLE_LOG() _leaf_log = false;
#define __LEAF_ENABLE_LOG() _leaf_log = true;
#define __LEAF_CLEAR_SERIAL() _leaf_should_clear_serial = true;
#define __LEAF_DONT_CLEAR_SERIAL() _leaf_should_clear_serial = false;
#define __LEAF_FLUSH_SERIAL() _leaf_should_flush_serial = true;
#define __LEAF_DONT_FLUSH_SERIAL() _leaf_should_flush_serial = false;
#define __LEAF_FLUSH_TTY() _leaf_should_flush_tty = true;
#define __LEAF_DONT_FLUSH_TTY() _leaf_should_flush_tty = false;
#define __LEAF_ENABLE_PRE_LOG() _leaf_disable_pre_log = false;
#define __LEAF_DISABLE_PRE_LOG() _leaf_disable_pre_log = true;

#endif  // __CONFIG_H__
