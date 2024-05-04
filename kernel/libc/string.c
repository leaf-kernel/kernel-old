#include <arch/cpu/utils.h>
#include <libc/stdio/printf.h>
#include <libc/string.h>
#include <sys/leaf.h>

void *memcpy(void *dest, const void *src, size_t n) {
    char *d = (char *)dest;
    const char *s = (const char *)src;
    while(n-- > 0) {
        *d++ = *s++;
    }
    return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
    char *d = (char *)dest;
    const char *s = (const char *)src;

    if(d == s) {
        return dest;
    }

    if(d < s) {
        while(n-- > 0) {
            *d++ = *s++;
        }
    } else {
        d += n;
        s += n;
        while(n-- > 0) {
            *--d = *--s;
        }
    }

    return dest;
}

char *strcpy(char *dest, const char *src) {
    char *d = dest;
    const char *s = src;
    while((*d++ = *s++) != '\0')
        ;
    return dest;
}

char *strncpy(char *dest, const char *src, size_t n) {
    char *d = dest;
    const char *s = src;
    while(n-- > 0 && (*d++ = *s++) != '\0')
        ;
    if(n > 0) {
        *d = '\0';
    }

    return dest;
}

char *strcat(char *dest, const char *src) {
    char *d = dest;
    const char *s = src;
    while(*d != '\0') {
        d++;
    }
    while((*d++ = *s++) != '\0')
        ;
    return dest;
}

char *strncat(char *dest, const char *src, size_t n) {
    char *d = dest;
    const char *s = src;
    while(*d != '\0') {
        d++;
    }
    while(n-- > 0 && (*d++ = *s++) != '\0')
        ;
    if(n > 0) {
        *d = '\0';
    }
    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;
    while(n-- > 0) {
        if(*p1 != *p2) {
            return *p1 - *p2;
        }
        p1++;
        p2++;
    }

    return 0;
}

int strcmp(const char *s1, const char *s2) {
    while(*s1 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }

    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    while(n && *s1 && (*s1 == *s2)) {
        ++s1;
        ++s2;
        --n;
    }
    if(n == 0) {
        return 0;
    } else {
        return (*(uint8_t *)s1 - *(uint8_t *)s2);
    }
}

void *memchr(const void *s, int c, size_t n) {
    const unsigned char *p = (const unsigned char *)s;
    while(n-- > 0) {
        if(*p == (unsigned char)c) {
            return (void *)p;
        }
        p++;
    }
    return NULL;
}

char *strchr(const char *s, int c) {
    while(*s != '\0') {
        if(*s == (char)c) {
            return (char *)s;
        }
        s++;
    }
    return NULL;
}

size_t strcspn(const char *s1, const char *s2) {
    size_t len = 0;
    while(*s1 != '\0') {
        if(strchr(s2, *s1) != NULL) {
            return len;
        }
        s1++;
        len++;
    }
    return len;
}

char *strpbrk(const char *s1, const char *s2) {
    while(*s1 != '\0') {
        if(strchr(s2, *s1) != NULL) {
            return (char *)s1;
        }
        s1++;
    }
    return NULL;
}

char *strrchr(const char *s, int c) {
    const char *last = NULL;
    while(*s != '\0') {
        if(*s == (char)c) {
            last = s;
        }
        s++;
    }
    return (char *)last;
}

size_t strspn(const char *s1, const char *s2) {
    size_t len = 0;
    while(*s1 != '\0' && strchr(s2, *s1) != NULL) {
        s1++;
        len++;
    }
    return len;
}

char *strstr(const char *haystack, const char *needle) {
    size_t needle_len = strlen(needle);
    while(*haystack != '\0') {
        if(strncmp(haystack, needle, needle_len) == 0) {
            return (char *)haystack;
        }
        haystack++;
    }
    return NULL;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    while(n-- > 0) {
        *p++ = (unsigned char)c;
    }
    return s;
}

char *strtok(char *str, const char *delim) {
    static char *last_token = NULL;
    if(str != NULL) {
        last_token = str;
    } else if(last_token == NULL) {
        return NULL;
    }
    char *token_start = last_token + strspn(last_token, delim);
    if(*token_start == '\0') {
        last_token = NULL;
        return NULL;
    }
    char *token_end = token_start + strcspn(token_start, delim);
    if(*token_end != '\0') {
        *token_end = '\0';
        last_token = token_end + 1;
    } else {
        last_token = NULL;
    }
    return token_start;
}

size_t strlen(const char *s) {
    const char *p = s;
    while(*p != '\0') {
        p++;
    }
    return (size_t)(p - s);
}

char *strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *new_s = (char *)kmalloc(len);
    if(new_s == NULL) {
        return NULL;
    }
    return strcpy(new_s, s);
}

void strrev(char *str) {
    int length = strlen(str);
    for(int i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}