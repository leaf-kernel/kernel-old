#include <libc/string.h>
#include <stddef.h>

size_t strlen(const char *str)
{
    size_t n = 0;
    while (str[n] != '\0')
    {
        n++;
    }
    return n;
}

long strtol(const char *nptr, char **endptr, int base)
{
    long result = 0;
    int sign = 1;

    while (*nptr == ' ' || (*nptr >= '\t' && *nptr <= '\r'))
    {
        nptr++;
    }

    if (*nptr == '-' || *nptr == '+')
    {
        sign = (*nptr++ == '-') ? -1 : 1;
    }

    if (base == 0)
    {
        base = (*nptr == '0' && (nptr[1] == 'x' || nptr[1] == 'X')) ? 16 : 10;
    }

    while ((*nptr >= '0' && *nptr <= '9') ||
           (base == 16 &&
            ((*nptr >= 'a' && *nptr <= 'f') || (*nptr >= 'A' && *nptr <= 'F'))))
    {
        int digit = *nptr - '0';
        if (base == 16 && *nptr >= 'a')
        {
            digit = (*nptr - 'a') + 10;
        }
        else if (base == 16 && *nptr >= 'A')
        {
            digit = (*nptr - 'A') + 10;
        }

        result = result * base + digit;
        nptr++;
    }

    if (endptr != NULL)
    {
        *endptr = (char *)nptr;
    }

    return result * sign;
}

char *strdup(const char *src)
{
    size_t length = strlen(src);
    char *dst = kmalloc(length + 1);
    if (dst != NULL)
    {
        strncpy(dst, src, length + 1);
    }
    return dst;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    while (n && *s1 && (*s1 == *s2))
    {
        ++s1;
        ++s2;
        --n;
    }
    if (n == 0)
    {
        return 0;
    }
    else
    {
        return (*(unsigned char *)s1 - *(unsigned char *)s2);
    }
}

char *strncpy(char *dest, const char *src, size_t n)
{
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    for (; i < n; i++)
    {
        dest[i] = '\0';
    }
    return dest;
}

char *strcpy(char *dest, const char *src)
{
    char *start = dest;
    while ((*dest++ = *src++) != '\0')
        ;
    return start;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2)
    {
        int diff = tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
        if (diff != 0)
        {
            return diff;
        }
        s1++;
        s2++;
    }

    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

uint64_t strtoul(const char *str, char **endptr, int base)
{
    uint64_t result = 0;
    int digit;

    while (isspace((unsigned char)*str))
    {
        str++;
    }

    int sign = 1;
    if (*str == '+' || *str == '-')
    {
        sign = (*str++ == '+') ? 1 : -1;
    }

    if ((base == 0 || base == 16) && *str == '0' &&
        (str[1] == 'x' || str[1] == 'X'))
    {
        str += 2;
        base = 16;
    }
    else if (base == 0 && *str == '0')
    {
        str++;
        base = 8;
    }

    while (isxdigit((unsigned char)*str) &&
           (digit = isdigit((unsigned char)*str)
                        ? *str - '0'
                        : (islower((unsigned char)*str)
                               ? tolower((unsigned char)*str) - 'a' + 10
                               : tolower((unsigned char)*str) - 'A' + 10)) <
               base)
    {
        result = result * base + digit;
        str++;
    }

    if (endptr != NULL)
    {
        *endptr = (char *)str;
    }

    return sign * result;
}

char *strtok(char *str, const char *delim)
{
    static char *next_token = NULL;
    char *token_start;

    if (str != NULL)
    {
        next_token = str;
    }
    else if (next_token == NULL)
    {
        return NULL;
    }

    while (*next_token != '\0' && strchr(delim, *next_token) != NULL)
    {
        next_token++;
    }

    if (*next_token == '\0')
    {
        next_token = NULL;
        return NULL;
    }

    token_start = next_token;

    while (*next_token != '\0' && strchr(delim, *next_token) == NULL)
    {
        next_token++;
    }

    if (*next_token != '\0')
    {
        *next_token = '\0';
        next_token++;
    }

    return token_start;
}

uint64_t octal_str_to_uint64(const char *str) { return strtoul(str, NULL, 8); }

int isdigit(unsigned char c) { return (c >= '0' && c <= '9'); }

int isxdigit(unsigned char c)
{
    return (isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

int isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
            c == '\f');
}

int islower(int c) { return (c >= 'a' && c <= 'z'); }

char tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c + ('a' - 'A');
    }
    else
    {
        return c;
    }
}

char *strrchr(const char *s, int c)
{
    const char *last_occurrence = NULL;
    // Find the first occurrence of c in s
    while (*s != '\0')
    {
        if (*s == c)
        {
            last_occurrence = s;
        }
        s++;
    }
    // If c was found, return a pointer to the last occurrence
    // Otherwise, return NULL
    return (char *)last_occurrence;
}
char *strchr(const char *str, int c)
{
    while (*str != '\0')
    {
        if (*str == c)
        {
            return (char *)str; // Cast const away to return non-const pointer
        }
        str++;
    }
    return NULL;
}