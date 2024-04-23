#ifndef __LEAF_PRIVATE_H__
#define __LEAF_PRIVATE_H__

// Functions to get stuff lol
void *__LEAF_GET_INITRD__();
void *__LEAF_GET_VFS__();

// Wrappers for cdebug_log and debug_log
#ifdef __LEAF_DEBUG_WRAPPERS__
#include <sys/logger.h>
#define dlog(...) debug_log(__FILE__, __LINE__, __func__, __VA_ARGS__);
#define cdlog(...) cdebug_log(__func__, __VA_ARGS__);
#define pdlog(...) pdebug_log(__FILE__, __LINE__, __func__, __VA_ARGS__);
#define pcdlog(...) pcdebug_log(__func__, __VA_ARGS__);
#endif

#endif // __LEAF_PRIVATE_H__