#include <utils/check.h>

bool __test_memcpy() {
    char dest[20];
    const char src[] = "Hello, world!";
    return strcmp(memcpy(dest, src, sizeof(src)), src) == 0;
}

bool __test_memmove() {
    char src[] = "Hello, World!";
    char dest[20];

    memmove(dest, src, strlen(src) + 1);

    if(strcmp(src, dest) != 0) {
        return false;
    }

    return true;
}

bool __test_strcpy() {
    char dest[20];
    const char src[] = "Hello, world!";
    return strcmp(strcpy(dest, src), src) == 0;
}

bool __test_strncpy() {
    char dest[20];
    const char src[] = "Hello, world!";
    strncpy(dest, src, 5);
    dest[5] = '\0';
    return strcmp(dest, "Hello") == 0;
}

bool __test_strcat() {
    char dest[20] = "Hello, ";
    const char src[] = "world!";
    return strcmp(strcat(dest, src), "Hello, world!") == 0;
}

bool __test_strncat() {
    char dest[20] = "Hello, ";
    const char src[] = "world!";
    strncat(dest, src, 3);
    return strcmp(dest, "Hello, wor") == 0;
}

bool __test_memcmp() {
    const char str1[] = "Hello";
    const char str2[] = "Hello";
    return memcmp(str1, str2, 4) == 0;
}

bool __test_strcmp() {
    const char str1[] = "Hello";
    const char str2[] = "Hello";
    return strcmp(str1, str2) == 0;
}

bool __test_strncmp() {
    const char str1[] = "Hello";
    const char str2[] = "Hello";
    return strncmp(str1, str2, 4) == 0;
}

bool __test_memchr() {
    const char str[] = "Hello, world!";
    return (memchr(str, 'o', strlen(str)) != NULL) &&
           strcmp(memchr(str, 'o', strlen(str)), "o, world!") == 0;
}

bool __test_strchr() {
    const char str[] = "Hello, world!";
    return (strchr(str, 'o') != NULL) &&
           strcmp(strchr(str, 'o'), "o, world!") == 0;
}

bool __test_strcspn() {
    const char str[] = "Hello, world!";
    return strcspn(str, ",") == 5;
}

bool __test_strpbrk() {
    const char str[] = "Hello, world!";
    return (strpbrk(str, "o") != NULL) &&
           strcmp(strpbrk(str, "o"), "o, world!") == 0;
}

bool __test_strrchr() {
    const char str[] = "Hello, world!";
    return (strrchr(str, 'o') != NULL) &&
           strcmp(strrchr(str, 'o'), "orld!") == 0;
}

bool __test_strspn() {
    const char str[] = "Hello, world!";
    return strspn(str, "Helo") == 5;
}

bool __test_strstr() {
    const char haystack[] = "Hello, world!";
    const char needle[] = "world";
    return (strstr(haystack, needle) != NULL) &&
           strcmp(strstr(haystack, needle), "world!") == 0;
}

bool __test_memset() {
    int arr[10];
    int value = 42;
    size_t num_bytes = sizeof(arr);

    memset(arr, value, num_bytes);

    unsigned char *byte_ptr = (unsigned char *)arr;
    for(size_t i = 0; i < num_bytes; i++) {
        if(byte_ptr[i] != (unsigned char)value) {
            return false;
        }
    }

    return true;
}

bool __test_strtok() {
    char str[] = "Hello, world!";
    const char delim[] = ",";
    char *token = strtok(str, delim);
    return (token != NULL) && strcmp(token, "Hello") == 0;
}

bool __test_strlen() {
    const char str[] = "Hello, world!";
    return strlen(str) == 13;
}

bool __test_strdup() {
    const char str[] = "Hello, world!";
    char *dup_str = strdup(str);
    return (dup_str != NULL) && strcmp(dup_str, str) == 0;
}

bool __test_isalnum() { return isalnum('a'); }

bool __test_isalpha() { return isalpha('a'); }

bool __test_isdigit() { return isdigit('5'); }

bool __test_isxdigit() { return isxdigit('F'); }

bool __test_islower() { return islower('a'); }

bool __test_isupper() { return isupper('A'); }

bool __test_isspace() { return isspace(' '); }

bool __test_ispunct() { return ispunct('.'); }

bool __test_isgraph() { return isgraph('a'); }

bool __test_isprint() { return isprint('a'); }

bool __test_iscntrl() { return iscntrl('\n'); }

bool __test_tolower() { return tolower('A') == 'a'; }

bool __test_toupper() { return toupper('a') == 'A'; }

bool __test_kmalloc() {
    void *ptr = kmalloc(16);
    if(ptr == NULL)
        return false;

    memset(ptr, 'A', 16);
    kfree(ptr);

    return true;
}

bool __test_kcalloc() {
    void *ptr = kcalloc(10);
    if(ptr == NULL)
        return false;

    bool is_zero_initialized = true;
    for(size_t i = 0; i < 10; ++i) {
        if(*((char *)ptr + i) != 0) {
            is_zero_initialized = false;
            break;
        }
    }

    kfree(ptr);

    return is_zero_initialized;
}

bool __test_krealloc() {
    void *ptr = kmalloc(8);
    if(ptr == NULL)
        return false;

    ptr = krealloc(ptr, 16);
    if(ptr == NULL)
        return false;

    kfree(ptr);

    return true;
}

bool __test_kfree() {
    void *ptr = kmalloc(8);
    if(ptr == NULL)
        return false;

    kfree(ptr);

    return true;
}

TestResult __check_ctype() {
    TestResult result = {0, 0};

    struct Test {
        const char *name;
        bool (*function)();
    };

    const struct Test tests[] = {
        {"isalnum", __test_isalnum}, {"isalpha", __test_isalpha},
        {"isdigit", __test_isdigit}, {"isxdigit", __test_isxdigit},
        {"islower", __test_islower}, {"isupper", __test_isupper},
        {"isspace", __test_isspace}, {"ispunct", __test_ispunct},
        {"isgraph", __test_isgraph}, {"isprint", __test_isprint},
        {"iscntrl", __test_iscntrl}, {"tolower", __test_tolower},
        {"toupper", __test_toupper}};

    for(size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i) {
        const struct Test *test = &tests[i];
        bool result_code = test->function();

        if(result_code) {
            vvok("%s: passed", test->name);
            result.passed++;
        } else {
            vvok("%s: failed", test->name);
            result.failed++;
        }
    }

    return result;
}

TestResult __check_string() {
    TestResult result = {0, 0};

    struct Test {
        const char *name;
        bool (*function)();
    };

    const struct Test tests[] = {
        {"memcpy", __test_memcpy},   {"memmove", __test_memmove},
        {"strcpy", __test_strcpy},   {"strncpy", __test_strncpy},
        {"strcat", __test_strcat},   {"strncat", __test_strncat},
        {"memcmp", __test_memcmp},   {"strcmp", __test_strcmp},
        {"strncmp", __test_strncmp}, {"memchr", __test_memchr},
        {"strchr", __test_strchr},   {"strcspn", __test_strcspn},
        {"strpbrk", __test_strpbrk}, {"strrchr", __test_strrchr},
        {"strspn", __test_strspn},   {"strstr", __test_strstr},
        {"memset", __test_memset},   {"strtok", __test_strtok},
        {"strlen", __test_strlen},   {"strdup", __test_strdup}};

    for(size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i) {
        const struct Test *test = &tests[i];
        bool result_code = test->function();

        if(result_code) {
            vvok("%s: passed", test->name);
            result.passed++;
        } else {
            vvok("%s: failed", test->name);
            result.failed++;
        }
    }

    return result;
}

TestResult __check_kheap() {
    TestResult result = {0, 0};

    struct Test {
        const char *name;
        bool (*function)();
    };

    const struct Test tests[] = {{"kmalloc", __test_kmalloc},
                                 {"kcalloc", __test_kcalloc},
                                 {"krealloc", __test_krealloc},
                                 {"kfree", __test_kfree}};

    for(size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i) {
        const struct Test *test = &tests[i];
        bool result_code = test->function();

        if(result_code) {
            vvok("%s: passed", test->name);
            result.passed++;
        } else {
            vvok("%s: failed", test->name);
            result.failed++;
        }
    }

    return result;
}

TestResult check_libc(bool verbose) {
    TestResult ctype_result;
    TestResult string_result;
    TestResult kheap_result;

    ctype_result = __check_ctype();
    string_result = __check_string();
    kheap_result = __check_kheap();

    if(verbose) {
        ok("<ctyp.h> result %d/%d", ctype_result.passed,
           ctype_result.failed + ctype_result.passed);
        ok("<string.h> result %d/%d", string_result.passed,
           string_result.failed + string_result.passed);
        ok("<stdlib.h> result %d/%d", kheap_result.passed,
           kheap_result.failed + kheap_result.passed);
    }

    TestResult total_result = {
        ctype_result.passed + string_result.passed + kheap_result.passed,
        ctype_result.failed + string_result.failed + kheap_result.failed};

    return total_result;
}
