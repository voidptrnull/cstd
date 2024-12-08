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
#include <stdlib.h>
#include <string.h>

#define LOAD_FACTOR_THRESHOLD 0.75
#define DELETED ((void *)-1)
#define CHASHSET_DEFAULT_CAPACITY 32

struct CHashSetEntry {
    void *key;
};

struct _CHashSet {
    struct CHashSetEntry *entries;
    uint64_t size;
    uint64_t capacity;
    uint64_t deleted_count;
    CompareTo cmp;
    Hash hash;
    Destructor destroyKey;
};

double CHashSet_load_factor(const CHashSet_t *set) {
    return set ? ((double)set->size / set->capacity) : 0.0;
}

static uint64_t __ceil(double x) {
    uint64_t int_part = (uint64_t)x;
    if (x > int_part) {
        return int_part + 1;
    }
    return int_part;
}

static int CHashSet_resize(CHashSet_t *set);

CResult_t *CHashSet_new(uint64_t capacity, CompareTo cmp, Hash hash,
                        Destructor destroyKey) {
    CHashSet_t *set = malloc(sizeof(CHashSet_t));
    if (!set)
        return CResult_ecreate(
            CError_create("Unable to allocate memory for hashset.",
                          "CHashSet_new", CHASHSET_ALLOC_FAILURE));

    int code = CHashSet_init(set, capacity, cmp, hash, destroyKey);
    if (code)
        return CResult_ecreate(CError_create("Unable to initialize hashset.",
                                             "CHashSet_create",
                                             CHASHSET_ALLOC_FAILURE));
    return CResult_create(set, NULL);
}

int CHashSet_init(CHashSet_t *set, uint64_t capacity, CompareTo cmp, Hash hash,
                  Destructor destroyKey) {
    if (!set || !cmp || !hash)
        return CHASHSET_NULL_SET;

    set->capacity = (capacity > 0) ? capacity : CHASHSET_DEFAULT_CAPACITY;
    set->size = 0;
    set->deleted_count = 0;
    set->cmp = cmp;
    set->hash = hash;
    set->destroyKey = destroyKey;

    set->entries = calloc(set->capacity, sizeof(struct CHashSetEntry));
    if (!set->entries)
        return CHASHSET_ALLOC_FAILURE;

    return CHASHSET_SUCCESS;
}

uint64_t CHashSet_size(const CHashSet_t *set) { return set ? set->size : 0; }

static int CHashSet_resize(CHashSet_t *set) {
    uint64_t new_capacity = __ceil(set->capacity * 1.5);
    struct CHashSetEntry *new_entries =
        calloc(new_capacity, sizeof(struct CHashSetEntry));
    if (!new_entries)
        return CHASHSET_ALLOC_FAILURE;

    uint64_t old_capacity = set->capacity;
    struct CHashSetEntry *old_entries = set->entries;

    set->entries = new_entries;
    set->capacity = new_capacity;
    set->size = 0;
    set->deleted_count = 0;

    for (uint64_t i = 0; i < old_capacity; i++) {
        if (old_entries[i].key && old_entries[i].key != DELETED) {
            uint64_t new_index = set->hash(old_entries[i].key) % new_capacity;
            while (new_entries[new_index].key != NULL) {
                new_index = (new_index + 1) % new_capacity;
            }
            new_entries[new_index] = old_entries[i];
            set->size++;
        }
    }

    free(old_entries);
    return CHASHSET_SUCCESS;
}

int CHashSet_add(CHashSet_t *set, void *key) {
    if (!set)
        return CHASHSET_NULL_SET;
    if (!key)
        return CHASHSET_NULL_KEY;

    if (CHashSet_load_factor(set) > LOAD_FACTOR_THRESHOLD) {
        if (CHashSet_resize(set) != CHASHSET_SUCCESS)
            return CHASHSET_ALLOC_FAILURE;
    }

    uint64_t index = set->hash(key) % set->capacity;

    while (set->entries[index].key && set->entries[index].key != DELETED) {
        if (set->cmp(set->entries[index].key, key) == 0) {
            return CHASHSET_SUCCESS;
        }
        index = (index + 1) % set->capacity;
    }

    set->entries[index].key = key;
    set->size++;

    return CHASHSET_SUCCESS;
}

int CHashSet_contains(CHashSet_t *set, void *key) {
    if (!set)
        return CHASHSET_NULL_SET;
    if (!key)
        return CHASHSET_NULL_KEY;

    uint64_t index = set->hash(key) % set->capacity;

    while (set->entries[index].key) {
        if (set->entries[index].key != DELETED &&
            set->cmp(set->entries[index].key, key) == 0) {
            return CHASHSET_SUCCESS;
        }
        index = (index + 1) % set->capacity;
    }

    return CHASHSET_NOT_FOUND;
}

int CHashSet_remove(CHashSet_t *set, void *key) {
    if (!set)
        return CHASHSET_NULL_SET;
    if (!key)
        return CHASHSET_NULL_KEY;

    uint64_t index = set->hash(key) % set->capacity;

    while (set->entries[index].key) {
        if (set->entries[index].key != DELETED &&
            set->cmp(set->entries[index].key, key) == 0) {
            if (set->destroyKey)
                set->destroyKey(set->entries[index].key);

            set->entries[index].key = DELETED;
            set->size--;
            set->deleted_count++;
            return CHASHSET_SUCCESS;
        }
        index = (index + 1) % set->capacity;
    }

    return CHASHSET_NOT_FOUND;
}

int CHashSet_clear(CHashSet_t *set) {
    if (!set)
        return CHASHSET_NULL_SET;

    for (uint64_t i = 0; i < set->capacity; i++) {
        if (set->entries[i].key && set->entries[i].key != DELETED) {
            if (set->destroyKey)
                set->destroyKey(set->entries[i].key);
        }
    }

    free(set->entries);
    set->capacity = 0;
    set->size = 0;
    set->deleted_count = 0;
    set->entries = NULL;

    return CHASHSET_SUCCESS;
}

int CHashSet_free(CHashSet_t **set) {
    if (!set)
        return CHASHSET_NULL_SET;

    for (uint64_t i = 0; i < (*set)->capacity; i++) {
        if ((*set)->entries[i].key && (*set)->entries[i].key != DELETED) {
            if ((*set)->destroyKey)
                (*set)->destroyKey((*set)->entries[i].key);
        }
    }

    free((*set)->entries);
    free(*set);
    *set = NULL;

    return CHASHSET_SUCCESS;
}
