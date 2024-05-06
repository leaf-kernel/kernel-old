#include <utils/check.h>

bool __test_memcpy() {
    char dest[20];
    const char src[] = "Hello, world!";
    return strcmp(memcpy(dest, src, sizeof(src)), src) == 0;
}

bool __test_memmove() {
    char str[] = "Hello, world!";
    return strcmp(memmove(str + 7, str, 5), "Hello, Hello!") == 0;
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
    const char str2[] = "Hella";
    return memcmp(str1, str2, 5) < 0;
}

bool __test_strcmp() {
    const char str1[] = "Hello";
    const char str2[] = "Hello";
    return strcmp(str1, str2) == 0;
}

bool __test_strncmp() {
    const char str1[] = "Hello";
    const char str2[] = "Hella";
    return strncmp(str1, str2, 5) < 0;
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
    char buf[20];
    return strcmp(memset(buf, 'A', sizeof(buf)), "AAAAAAAAAAAAAAAAAAAA") == 0;
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
        int (*function)(int);
    };

    const struct Test tests[] = {
        {"isalnum", isalnum},   {"isalpha", isalpha}, {"isdigit", isdigit},
        {"isxdigit", isxdigit}, {"islower", islower}, {"isupper", isupper},
        {"isspace", isspace},   {"ispunct", ispunct}, {"isgraph", isgraph},
        {"isprint", isprint},   {"iscntrl", iscntrl}, {"tolower", tolower},
        {"toupper", toupper}};

    for(size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i) {
        const struct Test *test = &tests[i];
        int result_code = test->function('a');

        if(result_code) {
            vcplog("%s: passed", test->name);
            result.passed++;
        } else {
            vcplog("%s: failed", test->name);
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
            vcplog("%s: passed", test->name);
            result.passed++;
        } else {
            vcplog("%s: failed", test->name);
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
            vcplog("%s: passed", test->name);
            result.passed++;
        } else {
            vcplog("%s: failed", test->name);
            result.failed++;
        }
    }

    return result;
}

TestResult check_libc() {
    TestResult ctype_result;
    TestResult string_result;
    TestResult kheap_result;

    ctype_result = __check_ctype();
    string_result = __check_string();
    kheap_result = __check_kheap();

    vcplog("ctype result %d/%d", ctype_result.passed,
           ctype_result.failed + ctype_result.passed);
    vcplog("string result %d/%d", string_result.passed,
           string_result.failed + string_result.passed);
    vcplog("kheap result %d/%d", kheap_result.passed,
           kheap_result.failed + kheap_result.passed);

    TestResult total_result = {
        ctype_result.passed + string_result.passed + kheap_result.passed,
        ctype_result.failed + string_result.failed + kheap_result.failed};

    return total_result;
}
