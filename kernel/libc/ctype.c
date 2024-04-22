#include <libc/ctype.h>

int isalnum(int c) {
    return (isalpha(c) || isdigit(c));
}

int isalpha(int c) {
    return (islower(c) || isupper(c));
}

int isdigit(int c) {
    return (c >= '0' && c <= '9');
}

int isxdigit(int c) {
    return (isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

int islower(int c) {
    return (c >= 'a' && c <= 'z');
}

int isupper(int c) {
    return (c >= 'A' && c <= 'Z');
}

int isspace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

int ispunct(int c) {
    return (isprint(c) && !isalnum(c) && !isspace(c));
}

int isgraph(int c) {
    return (isprint(c) && c != ' ');
}

int isprint(int c) {
    return (c >= 0x20 && c <= 0x7E);
}

int iscntrl(int c) {
    return (c < 0x20 || c == 0x7F);
}

int tolower(int c) {
    if (isupper(c)) {
        return c + ('a' - 'A');
    } else {
        return c;
    }
}

int toupper(int c) {
    if (islower(c)) {
        return c - ('a' - 'A');
    } else {
        return c;
    }
}
