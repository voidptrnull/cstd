#include <cstd/CLinkedList.h>
#include <stdlib.h>

typedef struct __CSN {
    void *value;
    struct __CSN *next;
} __CSNode;

typedef struct __CDN {
    void *value;
    struct __CDN *prev;
    struct __CDN *next;
} __CDNode;

typedef struct _CLinkedList {
    union {
        __CSNode *shead;
        __CDNode *dhead;
    };
    __CDNode *tail;
    Destructor destroy;
    size_t size;
} CLinkedList_t;

CResult_t *CLinkedList_new(int list_type, Destructor destroy) {
    CLinkedList_t *list = malloc(sizeof(CLinkedList_t));
    if (!list) {
        return CResult_ecreate(
            CError_create("Unable to allocate memory for linked list.",
                          "CLinkedList_new", CLINKEDLIST_ALLOC_FAILURE));
    }

    int code = CLinkedList_init(list, list_type, destroy);
    if (code) {
        free(list);
        return CResult_ecreate(CError_create(
            "Unable to initialize linked list.", "CLinkedList_new", code));
    }

    return CResult_create(list, NULL);
}

int CLinkedList_init(CLinkedList_t *list, int list_type, Destructor destroy) {
    if (!list) {
        return CLINKEDLIST_NULL_LIST;
    }

    list->destroy = destroy;
    list->size = 0;

    if (list_type) { // DOUBLY LINKED LIST
        list->dhead = malloc(sizeof(__CDNode));
        list->tail = malloc(sizeof(__CDNode));
        if (!list->dhead || !list->tail) {
            free(list->dhead);
            free(list->tail);
            return CLINKEDLIST_ALLOC_FAILURE;
        }

        list->dhead->next = list->tail;
        list->dhead->prev = NULL;
        list->dhead->value = NULL;

        list->tail->next = NULL;
        list->tail->prev = list->dhead;
        list->tail->value = NULL;

    } else { // SINGLY LINKED LIST
        list->shead = NULL;
        list->tail = NULL;
    }

    return CLINKEDLIST_SUCCESS;
}

int CLinkedList_add(CLinkedList_t *list, void *element) {
    if (!list) {
        return CLINKEDLIST_NULL_LIST;
    }

    if (list->tail) { // DOUBLY LINKED LIST
        __CDNode *new_node = malloc(sizeof(__CDNode));
        if (!new_node) {
            return CLINKEDLIST_ALLOC_FAILURE;
        }

        new_node->value = element;
        new_node->next = list->tail;
        new_node->prev = list->tail->prev;

        if (list->tail->prev) {
            list->tail->prev->next = new_node;
        } else {
            list->dhead = new_node;
        }
        list->tail->prev = new_node;

    } else { // SINGLY LINKED LIST
        __CSNode *new_node = malloc(sizeof(__CSNode));
        if (!new_node) {
            return CLINKEDLIST_ALLOC_FAILURE;
        }
        new_node->value = element;
        new_node->next = NULL;

        if (list->shead) {
            __CSNode *current = list->shead;
            while (current->next) {
                current = current->next;
            }
            current->next = new_node;
        } else {
            list->shead = new_node;
        }
    }

    list->size++;
    return CLINKEDLIST_SUCCESS;
}

int CLinkedList_remove(CLinkedList_t *list, size_t index) {
    if (!list) {
        return CLINKEDLIST_NULL_LIST;
    }

    if (index >= list->size) {
        return CLINKEDLIST_INDEX_OUT_OF_BOUNDS;
    }

    if (list->tail) { // DOUBLY LINKED LIST
        __CDNode *current;
        if (index < list->size / 2) {
            current = list->dhead->next;
            for (size_t i = 0; i < index; i++) {
                current = current->next;
            }
        } else {
            current = list->tail->prev;
            for (size_t i = list->size - 1; i > index; i--) {
                current = current->prev;
            }
        }

        if (current->prev) {
            current->prev->next = current->next;
        } else {
            list->dhead = current->next;
        }
        if (current->next) {
            current->next->prev = current->prev;
        } else {
            list->tail->prev = current->prev;
        }

        free(current);

    } else { // SINGLY LINKED LIST
        __CSNode *current = list->shead;
        __CSNode *prev = NULL;

        for (size_t i = 0; i < index; i++) {
            prev = current;
            current = current->next;
        }

        if (prev) {
            prev->next = current->next;
        } else {
            list->shead = current->next;
        }

        free(current);
    }

    list->size--;
    return CLINKEDLIST_SUCCESS;
}

CResult_t *CLinkedList_get(const CLinkedList_t *list, size_t index) {
    if (!list) {
        return CResult_ecreate(CError_create("List is NULL.", "CLinkedList_get",
                                             CLINKEDLIST_NULL_LIST));
    }

    if (index >= list->size) {
        return CResult_ecreate(CError_create("Index out of bounds.",
                                             "CLinkedList_get",
                                             CLINKEDLIST_INDEX_OUT_OF_BOUNDS));
    }

    if (list->tail) { // DOUBLY LINKED LIST
        __CDNode *current;
        if (index < list->size / 2) {
            current = list->dhead->next;
            for (size_t i = 0; i < index; i++) {
                current = current->next;
            }
        } else {
            current = list->tail->prev;
            for (size_t i = list->size - 1; i > index; i--) {
                current = current->prev;
            }
        }
        return CResult_create(current->value, NULL);
    } else { // SINGLY LINKED LIST
        __CSNode *current = list->shead;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
        return CResult_create(current->value, NULL);
    }
}

size_t CLinkedList_find(const CLinkedList_t *list, void *key, CompareTo cmp) {
    if (!list) {
        return CLINKEDLIST_NULL_LIST;
    }

    if (list->tail) {                          
        __CDNode *current = list->dhead->next;
        for (size_t i = 0; i < list->size; i++) {
            if (cmp(current->value, key) == 0)
                return i;
            current = current->next;
        }
    } else { // Singly linked list
        __CSNode *current = list->shead;
        for (size_t i = 0; i < list->size; i++) {
            if (cmp(current->value, key) == 0)
                return i;
            current = current->next;
        }
    }

    return CLINKEDLIST_INDEX_OUT_OF_BOUNDS;
}

int CLinkedList_clear(CLinkedList_t *list) {
    if (!list) {
        return CLINKEDLIST_NULL_LIST;
    }

    if (list->tail) { // Doubly linked list
        __CDNode *current = list->dhead->next;
        while (current != list->tail) {
            __CDNode *next = current->next;
            if (list->destroy) {
                list->destroy(current->value);
            }
            free(current);
            current = next;
        }
        list->dhead->next = list->tail;
        list->tail->prev = list->dhead;
    } else { // Singly linked list
        __CSNode *current = list->shead;
        while (current) {
            __CSNode *next = current->next;
            if (list->destroy) {
                list->destroy(current->value);
            }
            free(current);
            current = next;
        }
        list->shead = NULL;
    }

    list->size = 0;
    return CLINKEDLIST_SUCCESS;
}

int CLinkedList_free(CLinkedList_t **list) {
    if (!list || !*list) {
        return CLINKEDLIST_NULL_LIST;
    }

    CLinkedList_clear(*list);
    if ((*list)->tail) { // Doubly linked list
        free((*list)->dhead);
        free((*list)->tail);
    } else {
        free((*list)->shead);
    }
    free(*list);
    *list = NULL;

    return CLINKEDLIST_SUCCESS;
}

CResult_t *CLinkedList_clone(const CLinkedList_t *source, CloneFn cloner) {
    if (!source) {
        return CResult_ecreate(CError_create("Source list is NULL.",
                                             "CLinkedList_clone",
                                             CLINKEDLIST_NULL_LIST));
    }

    CLinkedList_t *clone = malloc(sizeof(CLinkedList_t));
    if (!clone) {
        return CResult_ecreate(
            CError_create("Unable to allocate memory for clone.",
                          "CLinkedList_clone", CLINKEDLIST_ALLOC_FAILURE));
    }

    int init_result =
        CLinkedList_init(clone, source->tail != NULL, source->destroy);
    if (init_result != CLINKEDLIST_SUCCESS) {
        free(clone);
        return CResult_ecreate(
            CError_create("Failed to initialize cloned list.",
                          "CLinkedList_clone", init_result));
    }

    if (source->tail) { // Doubly linked list
        __CDNode *current = source->dhead->next;
        while (current != source->tail) {
            void *cloned_value =
                cloner ? cloner(current->value) : current->value;
            if (CLinkedList_add(clone, cloned_value) != CLINKEDLIST_SUCCESS) {
                CLinkedList_free(&clone);
                return CResult_ecreate(CError_create(
                    "Failed to add element to clone.", "CLinkedList_clone",
                    CLINKEDLIST_ALLOC_FAILURE));
            }
            current = current->next;
        }
    } else { // Singly linked list
        __CSNode *current = source->shead;
        while (current) {
            void *cloned_value =
                cloner ? cloner(current->value) : current->value;
            if (CLinkedList_add(clone, cloned_value) != CLINKEDLIST_SUCCESS) {
                CLinkedList_free(&clone);
                return CResult_ecreate(CError_create(
                    "Failed to add element to clone.", "CLinkedList_clone",
                    CLINKEDLIST_ALLOC_FAILURE));
            }
            current = current->next;
        }
    }

    return CResult_create(clone, NULL);
}

size_t CLinkedList_size(const CLinkedList_t *list) {
    if (!list) {
        return 0;
    }
    return list->size;
}
