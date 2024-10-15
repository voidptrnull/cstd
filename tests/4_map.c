#include <logger/CLog.h>
#include <util/CHashMap.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int int_compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

size_t int_hash(const void *key) {
    return (*(int *)key) % CHASHMAP_DEFAULT_CAPACITY;
}

int create_hash_map(CHashMap **map) {
    CLog(INFO, "create_hash_map()");
    CResult *res =
        CHashMap_new(ccompare_integer, cdefault_hash, free, free);
    assert(!CResult_is_error(res));
    if (map == NULL)
        assert(0);
    *map = CResult_get(res);
    CResult_free(&res);
    return 0;
}

int init_hash_map(CHashMap *map) {
    CLog(INFO, "init_hash_map()");
    
    assert(CHashMap_init(map, 20, int_compare, int_hash, free, free) ==
           CHASHMAP_SUCCESS);
    return 0;
}

void test_insert(CHashMap *map) {
    CLog(INFO, "test_insert()");
    for (int i = 0; i < 10; i++) {
        int *key = malloc(sizeof(int));
        int *value = malloc(sizeof(int));
        *key = i;
        *value = i * 10;
        int result = CHashMap_insert(map, key, value);
        assert(result == CHASHMAP_SUCCESS);
    }
}

void test_lookup(CHashMap *map) {
    CLog(INFO, "test_lookup()");
    for (int i = 0; i < 10; i++) {
        int *key = malloc(sizeof(int));
        *key = i;
        CResult *result = CHashMap_get(map, key);
        assert(!CResult_is_error(result));
        CLog(DEBUG, "Log: %d", *key * 10);
        CLog(DEBUG, "Log2: %d", *(int *)CResult_get(result));
        assert(*(int *)CResult_get(result) == *key * 10);
        CResult_free(&result);
        free(key);
    }
}

void test_update(CHashMap *map) {
    for (int i = 0; i < 10; i++) {
        CLog(INFO, "test_update()");
        int *key = malloc(sizeof(int));
        int *new_value = malloc(sizeof(int));
        *key = i;
        *new_value = i * 2;
        int result = CHashMap_update(map, key, new_value);
        assert(result == CHASHMAP_SUCCESS);
        free(key);
    }
}

void test_lookup_v2(CHashMap *map) {
    CLog(INFO, "test_lookup()");
    for (int i = 0; i < 10; i++) {
        int *key = malloc(sizeof(int));
        *key = i;
        CResult *result = CHashMap_get(map, key);
        assert(!CResult_is_error(result));
        CLog(DEBUG, "Log: %d", *key * 2);
        CLog(DEBUG, "Log2: %d", *(int *)CResult_get(result));
        assert(*(int *)CResult_get(result) == *key * 2);
        CResult_free(&result);
        free(key);
    }
}

void test_remove(CHashMap *map) {
    CLog(INFO, "test_remove()");
    for (int i = 0; i < 10; i++) {
        int *key = malloc(sizeof(int));
        *key = i;
        CLog(DEBUG, "Removing %d.", i);
        int result = CHashMap_remove(map, key);
        assert(result == CHASHMAP_SUCCESS);
        free(key);
    }
}

void test_clear(CHashMap *map) {

    CLog(INFO, "test_clear()");
    int result = CHashMap_clear(map);
    assert(result == CHASHMAP_SUCCESS);
}

void test_free(CHashMap **map) {
    CLog(INFO, "test_free()");
    int result = CHashMap_free(map);
    assert(result == CHASHMAP_SUCCESS);
}

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

int ccompare_integer(const void *a, const void *b) {
    const int *int_a = (const int *)a;
    const int *int_b = (const int *)b;
    return (*int_a > *int_b) - (*int_a < *int_b);
}

int main() {
    enable_debugging();
    disable_location();
    CHashMap *map;
    create_hash_map(&map);
    init_hash_map(map);
    test_insert(map);
    test_lookup(map);
    test_update(map);
    test_lookup_v2(map);
    test_remove(map);
    test_clear(map);
    test_free(&map);
    return 0;
}
