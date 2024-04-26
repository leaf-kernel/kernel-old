#ifndef __LEAF_PRIVATE_H__
#define __LEAF_PRIVATE_H__

#ifdef LEAF_INCLUDE_PRIVATE
#ifdef __LEAF_PRIVATE__

void *__LEAF_GET_INITRD__();
void *__LEAF_GET_VFS__();

#define __LEAF_VOID_REDIRECT__ (void)0

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
#else
#define dlog(...) __LEAF_VOID_REDIRECT__
#define cdlog(...) __LEAF_VOID_REDIRECT__
#define plog(...) __LEAF_VOID_REDIRECT__
#define cplog(...) __LEAF_VOID_REDIRECT__
#endif

#define VIRT_TO_PHYS(addr) ((uint64_t)(addr)-0xffff800000000000)
#define PHYS_TO_VIRT(addr) ((uint64_t)(addr) + 0xffff800000000000)

#define volatile_read8(x) (*(volatile uint8_t*)&(x))
#define volatile_read16(x) (*(volatile uint16_t*)&(x))
#define volatile_read32(x) (*(volatile uint32_t*)&(x))
#define volatile_read64(x) (*(volatile uint64_t*)&(x))

#define volatile_write8(x, y) (*(volatile uint8_t*)&(x) = (y))
#define volatile_write16(x, y) (*(volatile uint16_t*)&(x) = (y))
#define volatile_write32(x, y) (*(volatile uint32_t*)&(x) = (y))
#define volatile_write64(x, y) (*(volatile uint64_t*)&(x) = (y))

#define CONFIG_CPU_MAX 256

#endif
#endif

#endif // __LEAF_PRIVATE_H__
