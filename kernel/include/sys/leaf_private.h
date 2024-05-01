#ifndef __LEAF_PRIVATE_H__
#define __LEAF_PRIVATE_H__

#ifdef LEAF_INCLUDE_PRIVATE
#ifdef __LEAF_PRIVATE__

#include <sys/leaf.h>

#ifdef __LEAF_CONFIG__
#include <sys/_config.h>
#endif

void *__LEAF_GET_INITRD__();
void *__LEAF_GET_VFS__();

#define __LEAF_VOID_REDIRECT__ (void)0

#ifdef __LEAF_VVERBOSE__
#ifndef __LEAF_VERBOSE__
#define __LEAF_VERBOSE__
#endif
#endif

#ifdef __LEAF_DEBUG_WRAPPERS__
#include <sys/logger.h>
#include <libc/stdio/printf.h>
#define dlog(...) debug_log(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define cdlog(...) cdebug_log(__func__, __VA_ARGS__)
#define plog(...) pdebug_log(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define cplog(...) pcdebug_log(__func__, __VA_ARGS__)
#ifdef __LEAF_VERBOSE__
#define vcdlog(...) cdebug_log(__func__, __VA_ARGS__)
#else
#define vcdlog(...) __LEAF_VOID_REDIRECT__
#endif
#ifdef __LEAF_VVERBOSE__
#define vvcdlog(...) cdebug_log(__func__, __VA_ARGS__)
#else
#define vvcdlog(...) __LEAF_VOID_REDIRECT__
#endif
#else
#define dlog(...) __LEAF_VOID_REDIRECT__
#define cdlog(...) __LEAF_VOID_REDIRECT__
#define plog(...) __LEAF_VOID_REDIRECT__
#define cplog(...) __LEAF_VOID_REDIRECT__
#endif
#endif

#define VIRT_TO_PHYS(addr) ((uint64_t)(addr)-hhdm_offset)
#define PHYS_TO_VIRT(addr) ((uint64_t)(addr) + hhdm_offset)
#endif

#endif // __LEAF_PRIVATE_H__
