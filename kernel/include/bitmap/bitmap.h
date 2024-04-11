#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

void bitmap_set(uint8_t *bitmap, uint64_t bit);
void bitmap_clear(uint8_t *bitmap, uint64_t bit);
uint8_t bitmap_get(uint8_t *bitmap, uint64_t bit);

#endif // BITMAP_H