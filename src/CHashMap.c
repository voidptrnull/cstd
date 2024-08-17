#include <stdlib.h>
#include <string.h>
#include <CHashMap.h>

struct CHashMapEntry {
    void* key;
    void* value;
};

int CHashMap_init(CHashMap* map, size_t capacity, CompareTo cmp, Hash hash) {
    if (map == NULL || cmp == NULL || hash == NULL) return CHASHMAP_NULL_MAP;

    map->capacity = (capacity > 0) ? capacity : CHASHMAP_DEFAULT_CAPACITY;
    map->size = 0;
    map->cmp = cmp;
    map->hash = hash;

    CResult* res = CVector_new(map->capacity);
    if (CResult_is_error(res))
        return CResult_eget(res)->err_code;
    
    map->buckets = CResult_get(res);
    if (map->buckets == NULL) return CHASHMAP_ALLOC_FAILURE;

    for (size_t i = 0; i < map->capacity; i++) {
        CVector_init(&map->buckets[i], 4);
    }

    return CHASHMAP_SUCCESS;
}

int CHashMap_insert(CHashMap* map, void* key, void* value) {
    if (map == NULL) return CHASHMAP_NULL_MAP;
    if (key == NULL || value == NULL) return CHASHMAP_NULL_VAL;

    size_t bucket_index = map->hash(key) % map->capacity;
    CVector* bucket = &map->buckets[bucket_index];

    struct CHashMapEntry* entry = malloc(sizeof(struct CHashMapEntry));
    entry->key = key;
    entry->value = value;

    return CVector_add(bucket, &entry);
}

void* CHashMap_lookup(CHashMap* map, void* key) {
    if (map == NULL || key == NULL) return NULL;

    size_t bucket_index = map->hash(key) % map->capacity;
    CVector* bucket = &map->buckets[bucket_index];

    for (size_t i = 0; i < bucket->size; i++) {
        CResult* v = CVector_get(bucket, i);
        if (!CResult_is_error(v)) {
            struct CHashMapEntry* entry = CResult_get(v);
            if (map->cmp(key, entry->key) == 0) {
                return entry->value;
            }
        }
    }

    return NULL;
}

int CHashMap_remove(CHashMap* map, void* key) {
    if (map == NULL) return CHASHMAP_NULL_MAP;
    if (key == NULL) return CHASHMAP_NULL_VAL;

    size_t bucket_index = map->hash(key) % map->capacity;
    CVector* bucket = &map->buckets[bucket_index];

    for (size_t i = 0; i < bucket->size; i++) {
        CResult* v = CVector_get(bucket, i);
        if (!CResult_is_error(v)) {
            struct CHashMapEntry* entry = CResult_get(v);
            if (map->cmp(key, entry->key) == 0) {
                CVector_del(bucket, i);
                return CHASHMAP_SUCCESS;
            }
        }
    }

    return CHASHMAP_NOT_FOUND;
}

int CHashMap_free(CHashMap* map) {
    if (map == NULL) return CHASHMAP_SUCCESS;

    for (size_t i = 0; i < map->capacity; i++) {
        CVector* bucket = &map->buckets[i];
        for (size_t j = 0; j < bucket->size; j++) {
            CResult* res = CVector_get(bucket, j);
            if (!CResult_is_error(res)) {
                struct CHashMapEntry* entry = CResult_get(res);
                free(entry->key);
                free(entry->value);
            }            
        }
        CVector_free(bucket);
    }

    free(map->buckets);
    map->size = 0;
    return CHASHMAP_SUCCESS;
}

int CHashMap_update(CHashMap* map, const void* key, const void* new_value) {
    if (map == NULL || key == NULL) return CHASHMAP_NULL_VAL;

    size_t bucket_index = map->hash(key) % map->capacity;
    CVector* bucket = &map->buckets[bucket_index];

    for (size_t i = 0; i < bucket->size; i++) {
        CResult* v = CVector_get(bucket, i);
        if (!CResult_is_error(v)) {
            struct CHashMapEntry* entry = CResult_get(v);
            if (map->cmp(key, entry->key) == 0) {
            entry->value = (void*)new_value;
            return CHASHMAP_SUCCESS;
            }
        }
    }

    return CHASHMAP_NOT_FOUND;
}

int CHashMap_clear(CHashMap* map) {
    if (map == NULL) return CHASHMAP_NULL_MAP;

    for (size_t i = 0; i < map->capacity; i++) {
        CVector* bucket = &map->buckets[i];
        for (size_t j = 0; j < bucket->size; j++) {
            CResult* res = CVector_get(bucket, j);
            if (!CResult_is_error(res)) {
                struct CHashMapEntry* entry = CResult_get(res);
                free(entry->key);
                free(entry->value);
            }
        }
        CVector_clear(bucket);
    }

    map->size = 0;
    return CHASHMAP_SUCCESS;
}

inline size_t CHashMap_size(const CHashMap* map) {
    return map ? map->size : 0;
}

double CHashMap_load_factor(const CHashMap* map) {
    return map ? ((double)map->size / map->capacity) : 0.0;
}
