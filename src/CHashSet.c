#include <logger/CLog.h>
#include <stdlib.h>
#include <string.h>
#include <util/CHashSet.h>

typedef struct Node {
    void *value;
    struct Node *next;
} Node;

struct _CHashSet {
    size_t capacity;
    size_t size;
    Node **buckets;
    Hash hash;
    Destructor destroy;
};

CResult *CHashSet_create(size_t capacity, Hash hash,
                         Destructor destroy) {
    CHashSet *set = malloc(sizeof(CHashSet));
    if (!set) {
        return CResult_ecreate(
            CError_create("Failed to initialize the newly created CHashSet.",
                          "CHashSet_create", CHASHSET_ALLOC_FAILURE));
    }
    int k = 0;
    if ((k = CHashSet_init(set, capacity, hash, destroy)) != 0) {
        free(set);
        return CResult_ecreate(
            CError_create("Failed to initialize the newly created CHashSet.",
                          "CHashSet_create", k));
    }
    return CResult_create(set, free);
}

int CHashSet_init(CHashSet *set, size_t capacity, Hash hash,
                  Destructor destroy) {
    if (!set)
        return CHASHSET_NULL_HASHSET;
    set->capacity = capacity;
    set->size = 0;
    set->hash = hash;
    set->buckets = malloc(capacity * sizeof(Node *));
    set->destroy = destroy;

    if (!set->buckets) {
        return CHASHSET_ALLOC_FAILURE;
    }
    return CHASHSET_SUCCESS;
}

int CHashSet_add(CHashSet *set, void *value) {
    if (!set || !set->buckets || !value) {
        return CHASHSET_NULL_HASHSET;
    }

    size_t index = set->hash(value) % set->capacity;
    Node *current = set->buckets[index];
    while (current) {
        if (set->hash(current->value) == set->hash(value) &&
            current->value == value) {
            return CHASHSET_VALUE_PRESENT_OR_NOT_FOUND;
        }
        current = current->next;
    }

    Node *new_node = malloc(sizeof(Node));
    if (!new_node) {
        return CHASHSET_ALLOC_FAILURE;
    }
    new_node->value = value;
    new_node->next = set->buckets[index];
    set->buckets[index] = new_node;
    set->size++;

    return CHASHSET_SUCCESS;
}

int CHashSet_remove(CHashSet *set, void *value) {
    if (!set || !set->buckets || !value) {
        return CHASHSET_NULL_HASHSET;
    }

    size_t index = set->hash(value) % set->capacity;
    Node *current = set->buckets[index];
    Node *prev = NULL;

    while (current) {
        if (set->hash(current->value) == set->hash(value) &&
            current->value == value) {
            if (prev) {
                prev->next = current->next;
            } else {
                set->buckets[index] = current->next;
            }
            free(current);
            set->size--;
            return CHASHSET_SUCCESS;
        }
        prev = current;
        current = current->next;
    }

    return CHASHSET_INDEX_OUT_OF_BOUNDS;
}

int CHashSet_contains(CHashSet *set, void *value) {
    if (!set || !set->buckets || !value) {
        return CHASHSET_NULL_HASHSET;
    }

    size_t index = set->hash(value) % set->capacity;
    Node *current = set->buckets[index];
    while (current) {
        if (set->hash(current->value) == set->hash(value) &&
            current->value == value) {
            return CHASHSET_SUCCESS;
        }
        current = current->next;
    }

    return CHASHSET_INDEX_OUT_OF_BOUNDS;
}

CResult *CHashSet_get(CHashSet *set, size_t key) {
    if (!set || !set->buckets) {
        return CResult_ecreate(
            CError_create("Unable to operate on null hash sets.",
                          "CHashSet_get", CHASHSET_NULL_HASHSET));
    }

    size_t index = key % set->capacity;
    Node *current = set->buckets[index];

    while (current) {
        if (set->hash(current->value) == key) {
            return CResult_create(current->value, NULL);
        }
        current = current->next;
    }

    return CResult_ecreate(CError_create("Could not find the value for the specified key.", "CHashSet_get", CHASHSET_VALUE_PRESENT_OR_NOT_FOUND));
}


int CHashSet_clear(CHashSet *set) {
    if (!set || !set->buckets) {
        return CHASHSET_NULL_HASHSET;
    }

    for (size_t i = 0; i < set->capacity; ++i) {
        Node *current = set->buckets[i];
        while (current) {
            Node *to_free = current;
            current = current->next;
            if (!set->destroy) {
                set->destroy(to_free);
            }
        }
        set->buckets[i] = NULL;
    }
    set->size = 0;

    return CHASHSET_SUCCESS;
}


int CHashSet_free(CHashSet *set) {
    if (!set) {
        return CHASHSET_NULL_HASHSET;
    }

    CHashSet_clear(set);
    free(set->buckets);
    free(set);

    return CHASHSET_SUCCESS;
}
