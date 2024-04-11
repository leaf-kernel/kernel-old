#ifndef __MATH_H__
#define __MATH_H__

#define DIV_ROUND_UP(x, y) (x + (y - 1)) / y
#define ALIGN_UP(x, y) DIV_ROUND_UP(x, y) * y
#define ALIGN_DOWN(x, y) (x / y) * y

int abs(int x);

#endif // __MATH_H_