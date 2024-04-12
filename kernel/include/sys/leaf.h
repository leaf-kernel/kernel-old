#ifndef __LEAF_H__
#define __LEAF_H__

// Bootloader include
#ifdef LEAF_LIMINE
#include <limine/limine.h>
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

// Global Variables
extern volatile struct limine_framebuffer_request framebuffer_request;
extern volatile struct limine_hhdm_request hhdm_request;
extern struct limine_framebuffer *framebuffer;
extern uint64_t hhdm_offset;

#endif // __LEAF_H__