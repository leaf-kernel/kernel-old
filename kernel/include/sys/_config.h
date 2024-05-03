#ifndef __CONFIG_H__
#define __CONFIG_H__

#define __LEAF_VERBOSE__
#define __LEAF_VVERBOSE__
#define __LEAF_DEBUG_WRAPPERS__
#define __LEAF_DEBUG__

#define CONFIG_CPU_MAX 256

#include <stdbool.h>

extern bool _leaf_log;

#define __LEAF_DISABLE_LOG() _leaf_log = false;
#define __LEAF_ENABLE_LOG() _leaf_log = true;

#endif  // __CONFIG_H__
