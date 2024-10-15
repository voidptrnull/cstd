#include <stdlib.h>
#include <string.h>
#include <util/CHashMap.h>

#define LOAD_FACTOR_THRESHOLD 0.75
#define DELETED ((void *)-1) // Marker for deleted entries

struct CHashMapEntry {
    void *key;
    void *value;
};

struct _CHashMap {
    struct CHashMapEntry *entries; ///< Array of hash map entries.
    size_t size;     ///< Number of key-value pairs currently in the hash map.
    size_t capacity; ///< Number of slots allocated for the hash map.
    CompareTo cmp;   ///< Comparison function for keys.
    Hash hash;       ///< Hash function for keys.
    Destructor destroyKey;   ///< Destructor for freeing the keys.
    Destructor destroyValue; ///< Destructor for freeing the values.
};


static size_t __ceil(double x) {
    size_t int_part = (size_t)x;
    if (x > int_part) {
        return int_part + 1;
    }
    return int_part;
}

static int CHashMap_resize(CHashMap *map);

CResult *CHashMap_new() {
    CHashMap *map = malloc(sizeof(CHashMap));
    if (!map)
        return CResult_ecreate(
            CError_create("Unable to allocate memory for hashmap.",
                          "CHashMap_new", CHASHMAP_ALLOC_FAILURE));

    return CResult_create(map, NULL);
}

int CHashMap_init(CHashMap *map, size_t capacity, CompareTo cmp, Hash hash,
                  Destructor destroyKey, Destructor destroyValue) {
    if (!map || !cmp || !hash)
        return CHASHMAP_NULL_MAP;

    map->capacity = (capacity > 0) ? capacity : CHASHMAP_DEFAULT_CAPACITY;
    map->size = 0;
    map->cmp = cmp;
    map->hash = hash;
    map->destroyKey = destroyKey;
    map->destroyValue = destroyValue;

    map->entries = calloc(map->capacity, sizeof(struct CHashMapEntry));
    if (!map->entries)
        return CHASHMAP_ALLOC_FAILURE;

    for (size_t i = 0; i < map->capacity; i++) {
        map->entries[i].key = NULL;
        map->entries[i].value = NULL;
    }

    return CHASHMAP_SUCCESS;
}

size_t CHashMap_size(const CHashMap *map) { return map ? map->size : 0; }

static int CHashMap_resize(CHashMap *map) {
    size_t new_capacity = __ceil(map->capacity * 1.5);
    struct CHashMapEntry *new_entries =
        calloc(new_capacity, sizeof(struct CHashMapEntry));
    if (!new_entries)
        return CHASHMAP_ALLOC_FAILURE;

    // Rehash and move all valid entries
    for (size_t i = 0; i < map->capacity; i++) {
        struct CHashMapEntry *entry = &map->entries[i];
        if (entry->key && entry->key != DELETED) {
            size_t new_index = map->hash(entry->key) % new_capacity;
            while (new_entries[new_index].key != NULL) {
                new_index = (new_index + 1) % new_capacity; // Linear probing
            }
            new_entries[new_index] = *entry; // Move entry
        }
    }

    free(map->entries);
    map->entries = new_entries;
    map->capacity = new_capacity;

    return CHASHMAP_SUCCESS;
}

int CHashMap_insert(CHashMap *map, void *key, void *value) {
    if (!map || !key || !value)
        return CHASHMAP_NULL_VAL;

    if (CHashMap_load_factor(map) > LOAD_FACTOR_THRESHOLD) {
        if (CHashMap_resize(map) != CHASHMAP_SUCCESS)
            return CHASHMAP_ALLOC_FAILURE;
    }

    size_t index = map->hash(key) % map->capacity;

    // Linear probing for insertion
    while (map->entries[index].key && map->entries[index].key != DELETED) {
        if (map->cmp(map->entries[index].key, key) == 0) {
            // Key already exists, update value
            if (map->destroyValue)
                map->destroyValue(map->entries[index].value);
            map->entries[index].value = value;
            return CHASHMAP_SUCCESS;
        }
        index = (index + 1) % map->capacity;
    }

    // Insert new entry
    map->entries[index].key = key;
    map->entries[index].value = value;
    map->size++;

    return CHASHMAP_SUCCESS;
}

CResult *CHashMap_get(CHashMap *map, void *key) {
    if (!map || !key)
        return NULL;

    size_t index = map->hash(key) % map->capacity;

    // Linear probing for search
    while (map->entries[index].key) {
        if (map->entries[index].key != DELETED &&
            map->cmp(map->entries[index].key, key) == 0) {
            return CResult_create(map->entries[index].value, NULL);
        }
        index = (index + 1) % map->capacity;
    }

    return CResult_ecreate(
        CError_create("Key not found.", "CHashMap_get", CHASHMAP_NOT_FOUND));
}

int CHashMap_remove(CHashMap *map, void *key) {
    if (!map || !key)
        return CHASHMAP_NULL_VAL;

    size_t index = map->hash(key) % map->capacity;

    // Linear probing for deletion
    while (map->entries[index].key) {
        if (map->entries[index].key != DELETED &&
            map->cmp(map->entries[index].key, key) == 0) {
            if (map->destroyKey)
                map->destroyKey(map->entries[index].key);
            if (map->destroyValue)
                map->destroyValue(map->entries[index].value);

            // Mark as deleted
            map->entries[index].key = DELETED;
            map->entries[index].value = NULL;
            map->size--;
            return CHASHMAP_SUCCESS;
        }
        index = (index + 1) % map->capacity;
    }

    return CHASHMAP_NOT_FOUND;
}

int CHashMap_clear(CHashMap *map) {
    if (!map)
        return CHASHMAP_NULL_MAP;
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->entries[i].key && map->entries[i].key != DELETED) {
            if (map->destroyKey)
                map->destroyKey(map->entries[i].key);
            if (map->destroyValue)
                map->destroyValue(map->entries[i].value);
        }
    }

    free(map->entries);
    map->capacity = 0;
    map->size = 0;
    map->entries = NULL;

    return CHASHMAP_SUCCESS;
}

int CHashMap_free(CHashMap **map) {
    if (!map || !*map)
        return CHASHMAP_NULL_MAP;

    for (size_t i = 0; i < (*map)->capacity; i++) {
        if ((*map)->entries[i].key && (*map)->entries[i].key != DELETED) {
            if ((*map)->destroyKey)
                (*map)->destroyKey((*map)->entries[i].key);
            if ((*map)->destroyValue)
                (*map)->destroyValue((*map)->entries[i].value);
        }
    }

    free((*map)->entries);
    free(*map);
    *map = NULL;

    return CHASHMAP_SUCCESS;
}

double CHashMap_load_factor(const CHashMap *map) {
    return map ? ((double)map->size / map->capacity) : 0.0;
}

int CHashMap_update(CHashMap *map, void *key, void *new_value) {
    if (!map || !key || !new_value)
        return CHASHMAP_NULL_VAL;

    size_t index = map->hash(key) % map->capacity;

    while (map->entries[index].key) {
        if (map->entries[index].key != DELETED &&
            map->cmp(map->entries[index].key, key) == 0) {

            if (map->destroyValue)
                map->destroyValue(map->entries[index].value);

            map->entries[index].value = new_value;
            return CHASHMAP_SUCCESS;
        }
        index = (index + 1) % map->capacity;
    }

    return CHASHMAP_NOT_FOUND; // Key not found
}
