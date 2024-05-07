#ifndef __LEAF_H__
#define __LEAF_H__

#include <stdint.h>
#include <sys/run/runner.h>
#include <utils/hash.h>

#ifdef LEAF_INCLUDE_PRIVATE
#define __LEAF_PRIVATE__
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

// Helper functions
#define STRINGIZE_HELPER(x...) #x
#define STRINGIZE(x...) STRINGIZE_HELPER(x)
#define ARCH_INCLUDE(a) STRINGIZE(arch / LEAF_ARCH_RAW / a)

// Global Variables
extern volatile struct limine_framebuffer_request framebuffer_request;
extern volatile struct limine_module_request mod_request;
extern volatile struct limine_hhdm_request hhdm_request;
extern volatile struct limine_rsdp_request rsdp_request;
extern struct limine_framebuffer *framebuffer;
extern uint64_t hhdm_offset;

// Leaf config and structs
typedef struct {
    uint32_t magic;
    uint8_t version_major;
    uint8_t version_minor;
    uint8_t version_patch;
    char build[32];
    char kernel[32];
    char *cpu_vendor;
} __attribute((packed)) __LEAF_HDR;

// Helper functions
#define ARRAY_SUM(arr, len)                                                    \
    ({                                                                         \
        int sum = 0;                                                           \
        for(int i = 0; i < len; i++) {                                         \
            sum += arr[i];                                                     \
        }                                                                      \
        sum;                                                                   \
    })

int main(service_t *self, void *signature);

#endif  // __LEAF_H__
