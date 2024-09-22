#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <util/CHashMap.h>

struct CHashMap {
    CVector *buckets; ///< Array of buckets (vectors) to store hash map entries.
    size_t size;      ///< Number of key-value pairs currently in the hash map.
    size_t capacity;  ///< Number of buckets allocated for the hash map.
    CompareTo cmp;    ///< Comparison function for keys.
    Hash hash;        ///< Hash function for keys.
    Destructor destroyKey;   ///< Destructor for freeing the keys.
    Destructor destroyValue; ///< Destructor for freeing the values.
};

struct CHashMapEntry {
    void *key;
    void *value;
};

int CHashMap_init(CHashMap *map, size_t capacity, CompareTo cmp, Hash hash,
                  Destructor destroyKey, Destructor destroyValue) {
    if (map == NULL || cmp == NULL || hash == NULL) {
        return CHASHMAP_NULL_MAP;
    }

    map->capacity = (capacity > 0) ? capacity : CHASHMAP_DEFAULT_CAPACITY;
    map->size = 0;
    map->cmp = cmp;
    map->hash = hash;
    map->destroyKey = destroyKey;
    map->destroyValue = destroyValue;

    CResult *res = CVector_new(map->capacity, NULL);
    if (CResult_is_error(res)) {
        int code = CError_get_code(CResult_eget(res));
        CResult_free(&res);
        return code;
    }

    map->buckets = CResult_get(res);
    if (map->buckets == NULL) {
        return CHASHMAP_ALLOC_FAILURE;
    }

    for (size_t i = 0; i < map->capacity; i++) {
        CVector_init(&map->buckets[i], 4, free); // Change this as needed
    }

    return CHASHMAP_SUCCESS;
}

int CHashMap_insert(CHashMap *map, void *key, void *value) {
    if (map == NULL)
        return CHASHMAP_NULL_MAP;
    if (key == NULL || value == NULL)
        return CHASHMAP_NULL_VAL;

    size_t bucket_index = map->hash(key) % map->capacity;
    CVector *bucket = &map->buckets[bucket_index];

    struct CHashMapEntry *entry = malloc(sizeof(struct CHashMapEntry));
    if (!entry)
        return CHASHMAP_ALLOC_FAILURE;

    entry->key = key;
    entry->value = value;

    return CVector_add(bucket, &entry);
}

void *CHashMap_lookup(CHashMap *map, void *key) {
    if (map == NULL || key == NULL)
        return NULL;

    size_t bucket_index = map->hash(key) % map->capacity;
    CVector *bucket = &map->buckets[bucket_index];

    for (size_t i = 0; i < CVector_size(bucket); i++) {
        CResult *v = CVector_get(bucket, i);
        if (!CResult_is_error(v)) {
            struct CHashMapEntry *entry = CResult_get(v);
            if (map->cmp(key, entry->key) == 0) {
                return entry->value;
            }
        }
    }

    return NULL;
}

int CHashMap_remove(CHashMap *map, void *key) {
    if (map == NULL)
        return CHASHMAP_NULL_MAP;
    if (key == NULL)
        return CHASHMAP_NULL_VAL;

    size_t bucket_index = map->hash(key) % map->capacity;
    CVector *bucket = &map->buckets[bucket_index];

    for (size_t i = 0; i < CVector_size(bucket); i++) {
        CResult *v = CVector_get(bucket, i);
        if (!CResult_is_error(v)) {
            struct CHashMapEntry *entry = CResult_get(v);
            if (map->cmp(key, entry->key) == 0) {
                if (map->destroyKey)
                    map->destroyKey(entry->key);
                if (map->destroyValue)
                    map->destroyValue(entry->value);
                CVector_del(bucket, i);
                return CHASHMAP_SUCCESS;
            }
        }
        CResult_free(&v);
    }

    return CHASHMAP_NOT_FOUND;
}

int CHashMap_free(CHashMap **map) {
    if (map == NULL || *map == NULL)
        return CHASHMAP_NULL_MAP;

    for (size_t i = 0; i < (*map)->capacity; i++) {
        CVector *bucket = &(*map)->buckets[i];
        for (size_t j = 0; j < CVector_size(bucket); j++) {
            CResult *res = CVector_get(bucket, j);
            if (!CResult_is_error(res)) {
                struct CHashMapEntry *entry = CResult_get(res);
                if ((*map)->destroyKey)
                    (*map)->destroyKey(entry->key);
                if ((*map)->destroyValue)
                    (*map)->destroyValue(entry->value);
            }
            CResult_free(&res);
        }
        CVector_free(&bucket);
    }

    free((*map)->buckets);
    free(*map);
    *map = NULL;

    return CHASHMAP_SUCCESS;
}

int CHashMap_update(CHashMap *map, const void *key, const void *new_value) {
    if (map == NULL || key == NULL)
        return CHASHMAP_NULL_VAL;

    size_t bucket_index = map->hash(key) % map->capacity;
    CVector *bucket = &map->buckets[bucket_index];

    for (size_t i = 0; i < CVector_size(bucket); i++) {
        CResult *v = CVector_get(bucket, i);
        if (!CResult_is_error(v)) {
            struct CHashMapEntry *entry = CResult_get(v);
            if (map->cmp(key, entry->key) == 0) {
                if (map->destroyValue)
                    map->destroyValue(entry->value);
                entry->value = (void *)new_value;
                return CHASHMAP_SUCCESS;
            }
        }
    }

    return CHASHMAP_NOT_FOUND;
}

int CHashMap_clear(CHashMap *map) {
    if (map == NULL)
        return CHASHMAP_NULL_MAP;

    for (size_t i = 0; i < map->capacity; i++) {
        CVector *bucket = &map->buckets[i];
        for (size_t j = 0; j < CVector_size(bucket); j++) {
            CResult *res = CVector_get(bucket, j);
            if (!CResult_is_error(res)) {
                struct CHashMapEntry *entry = CResult_get(res);
                if (map->destroyKey)
                    map->destroyKey(entry->key);
                if (map->destroyValue)
                    map->destroyValue(entry->value);
            }
        }
        CVector_clear(bucket);
    }

    map->size = 0;
    return CHASHMAP_SUCCESS;
}

inline size_t CHashMap_size(const CHashMap *map) { return map ? map->size : 0; }

double CHashMap_load_factor(const CHashMap *map) {
    return map ? ((double)map->size / map->capacity) : 0.0;
}