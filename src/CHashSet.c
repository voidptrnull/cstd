#include <debug/debug.h>
#include <stdlib.h>
#include <string.h>
#include <util/CHashSet.h>

typedef struct Node {
    void *value;
    struct Node *next;
} Node;

struct CHashSet  {
    size_t capacity;
    size_t size;
    Node** buckets;
    Hash hash;
};

CResult *CHashSet_create(unsigned long int capacity, Hash hash) {
    CHashSet *set = malloc(sizeof(CHashSet));
    if (!set) {
        return CResult_ecreate(
            CError_create("Failed to initialize the newly created CHashSet.",
                          "CHashSet_create", CHASHSET_ALLOC_FAILURE));
    }
    int k = 0;
    if ((k = CHashSet_init(set, capacity, hash)) != 0) {
        free(set);
        return CResult_ecreate(
            CError_create("Failed to initialize the newly created CHashSet.",
                          "CHashSet_create", k));
    }
    return CResult_create(set);
}

int CHashSet_init(CHashSet *set, unsigned long int capacity, Hash hash) {
    if (!set)
        return CHASHSET_NULL_HASHSET;
    set->capacity = capacity;
    set->size = 0;
    set->hash = hash;
    set->buckets = calloc(capacity, sizeof(Node *));
    if (!set->buckets) {
        return CHASHSET_ALLOC_FAILURE;
    }
    return CHASHSET_SUCCESS;
}

int CHashSet_clear(CHashSet *set) {
    if (!set || !set->buckets) {
        return CHASHSET_NULL_HASHSET;
    }

    for (unsigned long int i = 0; i < set->capacity; ++i) {
        Node *current = set->buckets[i];
        while (current) {
            Node *to_free = current;
            current = current->next;
            free(to_free);
        }
        set->buckets[i] = NULL;
    }
    set->size = 0;

    return CHASHSET_SUCCESS;
}

int CHashSet_add(CHashSet *set, void *value) {
    if (!set || !set->buckets || !value) {
        return CHASHSET_NULL_HASHSET;
    }

    unsigned long int index = set->hash(value) % set->capacity;
    Node *current = set->buckets[index];
    while (current) {
        if (set->hash(current->value) == set->hash(value) &&
            current->value == value) {
            return CHASHSET_VALUE_PRESENT;
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

    unsigned long int index = set->hash(value) % set->capacity;
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

    unsigned long int index = set->hash(value) % set->capacity;
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

CResult *CHashSet_get(CHashSet *set, unsigned long int key) {
    if (!set || !set->buckets) {
        return CResult_ecreate(
            CError_create("Unable to operate on null hash sets.",
                          "CHashSet_get", CHASHSET_NULL_HASHSET));
    }

    unsigned long int index = key % set->capacity;
    Node *current = set->buckets[index];

    while (current) {
        if (set->hash(current->value) == key) {
            return current->value;
        }
        current = current->next;
    }

    return CResult_create(NULL);
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
