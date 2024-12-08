#include <cstd/CLog.h>
#include <cstd/CHashSet.h>

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
    CHashSet_t* set;
    CResult_t *res = CHashSet_new(20,int_compare, int_hash, free);
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
