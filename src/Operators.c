/*
 * MIT License
 *
 * Copyright (c) 2024 Subhadip Roy Chowdhury
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <cstd/Operators.h>

#ifndef CSTD_NO_DEF_FN_IMPL
#define HAVE_CSTD_DEFAULTS

#include <stdint.h>
#include <stdlib.h>

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

size_t cdefault_hash(const void *key) {
    uintptr_t ptr = (uintptr_t)key;
    ptr ^= (ptr >> 33);
    ptr *= 0xff51afd7ed558ccd;
    ptr ^= (ptr >> 33);
    ptr *= 0xc4ceb9fe1a85ec53;
    ptr ^= (ptr >> 33);

    return (size_t)ptr;
}

void *cclone_integer(const void *data) {
    if (data == NULL)
        return NULL;
    int *clone = malloc(sizeof(int));
    if (clone) {
        *clone = *(intmax_t *)data;
    }
    return clone;
}

#endif
