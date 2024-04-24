#ifndef __LEAF_PRIVATE_H__
#define __LEAF_PRIVATE_H__

#ifdef LEAF_INCLUDE_PRIVATE
#ifdef __LEAF_PRIVATE__

void *__LEAF_GET_INITRD__();
void *__LEAF_GET_VFS__();

#ifdef __LEAF_DEBUG_WRAPPERS__
#include <sys/logger.h>
#include <libc/stdio/printf.h>
#define dlog(...) debug_log(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define cdlog(...) cdebug_log(__func__, __VA_ARGS__)
#define plog(...) pdebug_log(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define cplog(...) pcdebug_log(__func__, __VA_ARGS__)
#endif

#define VIRT_TO_PHYS(addr) ((uint64_t)(addr)-0xffff800000000000)
#define PHYS_TO_VIRT(addr) ((uint64_t)(addr) + 0xffff800000000000)

#endif
#endif

#endif // __LEAF_PRIVATE_H__
