#include <ops/Operators.h>

#ifndef CSTD_NO_DEFAULT_FUNCTION_IMPLEMENTATIONS
#define HAVE_CSTD_DEFAULTS
int ccompare_pointer(const void *a, const void *b) { return (a > b) - (a < b); }

int ccompare_integer(const void *a, const void *b) {
    const int *int_a = (const int *)a;
    const int *int_b = (const int *)b;
    return (*int_a > *int_b) - (*int_a < *int_b);
}

size_t chash_string(const void *key) {
    const char *str_key = (const char *)key;
    size_t hash = 5381;
    int c;
    while ((c = *str_key++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

#include <stdint.h>
size_t cdefault_hash(const void *key) {
    uintptr_t ptr = (uintptr_t)key;
    ptr ^= (ptr >> 21);
    ptr ^= (ptr >> 35);
    ptr ^= (ptr >> 4);
    ptr *= 0xDAA4D7D8; // A random prime number for mixing
    ptr ^= (ptr >> 11);
    ptr ^= (ptr >> 17);

    return (size_t)ptr;
}

#endif
