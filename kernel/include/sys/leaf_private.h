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
#ifndef __LEAF_DEBUG__
#define __LEAF_DEBUG__
#endif
#endif

#ifdef __LEAF_VERBOSE__
#define vok(...) ok(__VA_ARGS__)
#ifdef __LEAF_VVERBOSE__
#define vvok(...) vok(__VA_ARGS__)
#else
#define vvok(...)
#endif
#ifdef __LEAF_VVVERBOSE__
#define vvvok(...) vvok(__VA_ARGS__)
#else
#define vvvok(...)
#endif
#else
#define vok(...)
#define vvok(...)
#define vvvok(...)
#endif
#endif

#define VIRT_TO_PHYS(addr) ((uint64_t)(addr) - hhdm_offset)
#define PHYS_TO_VIRT(addr) ((uint64_t)(addr) + hhdm_offset)
#endif

#endif  // __LEAF_PRIVATE_H__
