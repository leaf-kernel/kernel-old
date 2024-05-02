#ifndef __INI_H__
#define __INI_H__
#include <stdint.h>

struct key_value_pair {
    char *key;
    char *value;
};
struct parsed_ini {
    struct key_value_pair *data;
    uint8_t count;
};

struct parsed_ini parse_ini(char *string);

#endif  // __INI_H__