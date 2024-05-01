#include <utils/convertion.h>

uint64_t bytes_to_kb(uint64_t a) { return (a + 1023) / 1024; }

uint64_t bytes_to_mb(uint64_t a) {
    return (a + (1024ULL * 1024 - 1)) / (1024ULL * 1024);
}