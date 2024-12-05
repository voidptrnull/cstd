#include <cstd/CLog.h>
#include <cstd/CHashMap.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_MAX 1000

int int_compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int64_t int_hash(const void *key) { return (*(int *)key) % 4096 + 127; }

int create_hash_map(CHashMap_t **map) {
    CLog(INFO, "create_hash_map()");
    CResult_t *res =
        CHashMap_new(20, ccompare_integer, int_hash, free, free);
    assert(!CResult_is_error(res));
    assert(map);
    *map = CResult_get(res);
    CResult_free(&res);
    return 0;
}

void test_insert(CHashMap_t *map) {
    CLog(INFO, "test_insert()");
    for (int i = 0; i < TEST_MAX; i++) {
        int *key = malloc(sizeof(int));
        int *value = malloc(sizeof(int));
        *key = i * 200;
        *value = i;
        int result = CHashMap_insert(map, key, value);
        assert(result == CHASHMAP_SUCCESS);
    }
}

void test_lookup(CHashMap_t *map) {
    CLog(INFO, "test_lookup()");
    for (int i = 0; i < TEST_MAX; i++) {
        int key = i * 200;
        CResult_t *result = CHashMap_get(map, &key);
        assert(!CResult_is_error(result));
        assert(*(int *)CResult_get(result) == i);
        CResult_free(&result);
    }
}

void test_update(CHashMap_t *map) {
    CLog(INFO, "test_update()");
    for (int i = 0; i < TEST_MAX; i++) {
        int *key = malloc(sizeof(int));
        int *new_value = malloc(sizeof(int));
        *key = i * 200;
        *new_value = i * 2;
        int result = CHashMap_update(map, key, new_value);
        assert(result == CHASHMAP_SUCCESS);
        free(key);
    }
}

void test_lookup_v2(CHashMap_t *map) {
    CLog(INFO, "test_lookup()");
    for (int i = 0; i < TEST_MAX; i++) {
        int *key = malloc(sizeof(int));
        *key = i * 200;
        CResult_t *result = CHashMap_get(map, key);
        assert(!CResult_is_error(result));
        CLog(DEBUG, "Log: %d", *key * 2);
        CLog(DEBUG, "Log2: %d", *(int *)CResult_get(result));
        assert(*(int *)CResult_get(result) == *key / 100);
        CResult_free(&result);
        free(key);
    }
}

void test_remove(CHashMap_t *map) {
    CLog(INFO, "test_remove()");
    for (int i = 0; i < TEST_MAX; i++) {
        int *key = malloc(sizeof(int));
        *key = i * 200;
        CLog(DEBUG, "Removing %d.", i);
        int result = CHashMap_remove(map, key);
        assert(result == CHASHMAP_SUCCESS);
        free(key);
    }
}

void test_clear(CHashMap_t *map) {

    CLog(INFO, "test_clear()");
    int result = CHashMap_clear(map);
    assert(result == CHASHMAP_SUCCESS);
}

void test_free(CHashMap_t **map) {
    CLog(INFO, "test_free()");
    int result = CHashMap_free(map);
    assert(result == CHASHMAP_SUCCESS);
}

int64_t cdefault_hash(const void *key) {
    uintptr_t ptr = (uintptr_t)key;
    ptr ^= (ptr >> 21);
    ptr ^= (ptr >> 35);
    ptr ^= (ptr >> 4);
    ptr *= 0xD45F3; // A random prime number for mixing
    ptr ^= (ptr >> 11);
    ptr ^= (ptr >> 17);

    return (int64_t)ptr;
}

int ccompare_integer(const void *a, const void *b) {
    const int *int_a = (const int *)a;
    const int *int_b = (const int *)b;
    return (*int_a > *int_b) - (*int_a < *int_b);
}

int main() {
    //enable_debugging();
    enable_location();
    shortened_location();
    CHashMap_t *map;
    create_hash_map(&map);
    test_insert(map);
    test_lookup(map);
    test_update(map);
    test_lookup_v2(map);
    test_remove(map);
    test_clear(map);
    test_free(&map);
    return 0;
}
