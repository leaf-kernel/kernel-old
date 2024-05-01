#include <bitmap/bitmap.h>

void bitmap_set(uint8_t *bitmap, uint64_t bit) {
    bitmap[bit / 8] |= 1 << (bit % 8);
}

void bitmap_clear(uint8_t *bitmap, uint64_t bit) {
    bitmap[bit / 8] &= ~(1 << (bit % 8));
}

uint8_t bitmap_get(uint8_t *bitmap, uint64_t bit) {
    return bitmap[bit / 8] & (1 << (bit % 8));
}