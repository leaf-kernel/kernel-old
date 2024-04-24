#ifndef __LEAF_H__
#define __LEAF_H__

#include <utils/hash.h>
#include <stdint.h>

#ifdef LEAF_INCLUDE_PRIVATE
#define __LEAF_PRIVATE__
#ifdef LEAF_DEBUG
#define __LEAF_DEBUG_WRAPPERS__
#endif
#include <sys/leaf_private.h>
#endif

#ifdef LEAF_LIMINE
#include <sys/limine.h>
#endif

// Leaf return codes.
#define LEAF_RETURN_SUCCESS 0
#define LEAF_RETURN_FAIL 1
#define LEAF_RETURN_FATAL 2

// Leaf definitions
#ifndef LEAF_ARCH
#define LEAF_ARCH "Unknown"
#endif

#ifndef LEAF_BOOTLOADER
#define LEAF_BOOTLOADER "Unknown"
#endif

#ifndef LEAF_VERSION
#define LEAF_VERSION "Unknown"
#endif

#ifndef LEAF_UUID
#define LEAF_UUID "Unknown"
#endif

#ifndef LEAF_OFFSET
#define LEAF_OFFSET "69420"
#endif

// Helper functions
#define STRINGIZE_HELPER(x...) #x
#define STRINGIZE(x...) STRINGIZE_HELPER(x)
#define ARCH_INCLUDE(a) STRINGIZE(arch/LEAF_ARCH_RAW/a)

// Global Variables
extern volatile struct limine_framebuffer_request framebuffer_request;
extern volatile struct limine_module_request mod_request;
extern volatile struct limine_hhdm_request hhdm_request;
extern struct limine_framebuffer *framebuffer;
extern uint64_t hhdm_offset;

#endif // __LEAF_H__
