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

#include <cstd/CHashSet.h>
#include <cstd/CLog.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int int_compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

uint64_t int_hash(const void *key) { return (*(int *)key) + 1; }

void test_insert(CHashSet_t *set) {
    CLog(INFO, "test_insert()");
    for (int i = 0; i < 1000; i++) {
        int *value = malloc(sizeof(int));
        *value = i;
        int result = CHashSet_add(set, value);
        assert(result == CHASHSET_SUCCESS);
    }
}

void test_remove(CHashSet_t *set) {
    CLog(INFO, "test_remove()");
    for (int i = 0; i < 1000; i++) {
        int value = i;
        int result = CHashSet_remove(set, &value);
        assert(result == CHASHSET_SUCCESS);
    }
}

void test_lookup(CHashSet_t *set) {
    CLog(INFO, "test_lookup()");
    for (int i = 0; i < 1000; i++) {
        int value = i;
        int result = CHashSet_contains(set, &value);
        assert(result != CHASHSET_NOT_FOUND);
    }
}
void test_clear(CHashSet_t *set) {
    CLog(INFO, "test_clear()");
    int result = CHashSet_clear(set);
    assert(result == CHASHSET_SUCCESS);
}

void test_free(CHashSet_t **set) {
    CLog(INFO, "test_free()");
    int result = CHashSet_free(set);

    assert(result == CHASHSET_SUCCESS);
}

int main() {
    enable_debugging();
    enable_location();
    shortened_location();
    CLog(INFO, "create_hash_set()");
    CHashSet_t *set;
    CResult_t *res = CHashSet_new(20, int_compare, int_hash, free);
    assert(!CResult_is_error(res));
    set = CResult_get(res);
    test_insert(set);
    test_lookup(set);
    test_remove(set);
    test_clear(set);
    test_free(&set);
    CResult_free(&res);
    return 0;
}
