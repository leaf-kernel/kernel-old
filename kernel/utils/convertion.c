#include <utils/convertion.h>

uint64_t bytes_to_kib(uint64_t a) { return (a + 1023) / 1024; }

uint64_t bytes_to_mib(uint64_t a) {
    return (a + (1024ULL * 1024 - 1)) / (1024ULL * 1024);
}

uint64_t bytes_to_gib(uint64_t a) { return (a + 1023) / 1024 / 1024 / 1024; }

uint64_t bytes_to_kb(uint64_t a) { return (a + 999) / 1000; }

uint64_t bytes_to_mb(uint64_t a) { return (a + 999999) / 1000000; }

uint64_t bytes_to_gb(uint64_t a) { return (a + 999999999) / 1000000000; }
