#include <stdlib.h>
#include <string.h>
#include <util/CSLinkedList.h>

/// \struct CSNode
/// \brief Represents a node in a singly linked list.
/// \details The `CSNode` structure contains a pointer to the data and a pointer
/// to the next node in the list.
///          The `next` pointer is used to link nodes together in a singly
///          linked list.
struct CSNode {
    void *data;          ///" Pointer to the data stored in the node.
    struct CSNode *next; ///" Pointer to the next node in the list.
};

struct CSLinkedList {
    struct CSNode *node; ///" Pointer to the first node in the list.
    size_t size;         ///" The number of nodes in the list.
};

CResult *CSLinkedList_new() {
    CSLinkedList *list = malloc(sizeof(CSLinkedList));
    if (list == NULL) {
        return CResult_ecreate(
            CError_create("Unable to allocate memory for list.",
                          "CSLinkedList_new", LINKED_LIST_ALLOC_FAILURE));
    }

    int code = CSLinkedList_init(list);
    if (code) {
        free(list);
        return CResult_ecreate(
            CError_create("Unable to initialize the newly created list.",
                          "CSLinkedList_new", code));
    }
    return CResult_create(list);
}

int CSLinkedList_init(CSLinkedList *list) {
    if (list == NULL)
        return LINKED_LIST_NULL_LIST;

    list->node = NULL;
    list->size = 0;

    return LINKED_LIST_SUCCESS;
}

int CSLinkedList_add(CSLinkedList *list, void *data) {
    if (list == NULL)
        return LINKED_LIST_NULL_LIST;

    struct CSNode *node = malloc(sizeof(struct CSNode));
    if (node == NULL)
        return LINKED_LIST_ALLOC_FAILURE;

    node->data = data;
    node->next = NULL;

    if (list->node == NULL) {
        list->node = node;
    } else {
        struct CSNode *current = list->node;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }

    list->size++;
    return LINKED_LIST_SUCCESS;
}

int CSLinkedList_remove(CSLinkedList *list, size_t index) {
    if (list == NULL)
        return LINKED_LIST_NULL_LIST;
    if (list->node == NULL || list->size <= index)
        return LINKED_LIST_OUT_OF_BOUNDS;

    struct CSNode *node = list->node;
    struct CSNode *pnode = NULL;

    for (size_t i = 0; i < index; i++) {
        pnode = node;
        node = node->next;
    }

    if (pnode == NULL) {
        list->node = node->next;
    } else {
        pnode->next = node->next;
    }

    free(node);
    list->size--;
    return LINKED_LIST_SUCCESS;
}

size_t CSLinkedList_find(CSLinkedList *list, void *key, CompareTo cmp) {
    if (list == NULL || cmp == NULL)
        return LINKED_LIST_NULL_LIST;

    struct CSNode *node = list->node;
    size_t index = 0;

    while (node != NULL) {
        if (cmp(node->data, key) == 0) {
            return index;
        }
        node = node->next;
        index++;
    }

    return LINKED_LIST_NOT_FOUND;
}

CResult *CSLinkedList_get(const CSLinkedList *list, size_t index) {
    if (list == NULL)
        return CResult_ecreate(CError_create(
            "List is null.", "CSLinkedList_get", LINKED_LIST_NULL_LIST));
    if (list->size <= index)
        return CResult_ecreate(CError_create("Index out of bounds.",
                                             "CSLinkedList_get",
                                             LINKED_LIST_OUT_OF_BOUNDS));

    struct CSNode *node = list->node;
    for (size_t i = 0; i < index; i++) {
        node = node->next;
    }

    return CResult_create(node->data);
}

int CSLinkedList_set(CSLinkedList *list, size_t index, void *data) {
    if (list == NULL)
        return LINKED_LIST_NULL_LIST;
    if (list->size <= index)
        return LINKED_LIST_OUT_OF_BOUNDS;

    struct CSNode *node = list->node;
    for (size_t i = 0; i < index; i++) {
        node = node->next;
    }

    node->data = data;
    return LINKED_LIST_SUCCESS;
}

int CSLinkedList_clear(CSLinkedList *list) {
    if (list == NULL)
        return LINKED_LIST_NULL_LIST;

    struct CSNode *node = list->node;
    while (node != NULL) {
        struct CSNode *next = node->next;
        free(node);
        node = next;
    }

    list->node = NULL;
    list->size = 0;
    return LINKED_LIST_SUCCESS;
}

int CSLinkedList_free(CSLinkedList **list) {
    if (list == NULL || *list == NULL)
        return LINKED_LIST_SUCCESS;

    int code = CSLinkedList_clear(*list);
    if (code)
        return code;
    free(*list);
    *list = NULL;
    return LINKED_LIST_SUCCESS;
}
