#include <stdlib.h>
#include <util/CDLinkedList.h>

/// \struct CDNode
/// \brief Structure for storing data in the linked list.
/// \details The `CDNode` structure is used to represent each node in the doubly
/// linked list.
///          It contains a pointer to the data, and pointers to the previous and
///          next nodes. Initially, the previous and next pointers are set to
///          `NULL`.
struct CDNode {
    void *data;          ///< Pointer to the data stored in the node.
    struct CDNode *prev; ///< Pointer to the previous node in the list.
    struct CDNode *next; ///< Pointer to the next node in the list.
};

struct CDLinkedList {
    struct CDNode *head; ///< Pointer to the first node in the list.
    struct CDNode *tail; ///< Pointer to the last node in the list.
    size_t size;         ///< The number of nodes in the list.
};

CResult *CDLinkedList_new() {
    CDLinkedList *list = (CDLinkedList *)malloc(sizeof(CDLinkedList));
    if (list == NULL) {
        return CResult_ecreate(
            CError_create("Failed to allocate memory for CDLinkedList.",
                          "CDLinkedList_new", -1));
    }
    int code = CDLinkedList_init(list);
    if (code) {
        free(list);
        return CResult_ecreate(CError_create(
            "Failed to initialize CDLinkedList.", "CDLinkedList_new", code));
    }
    return CResult_create(list);
}

int CDLinkedList_init(CDLinkedList *list) {
    if (list == NULL)
        return -1;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return 0;
}

int CDLinkedList_add(CDLinkedList *list, void *data) {
    if (list == NULL)
        return LINKED_LIST_NULL_LIST;

    struct CDNode *new_node = (struct CDNode *)malloc(sizeof(struct CDNode));
    if (new_node == NULL)
        return LINKED_LIST_ALLOC_FAILURE;
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = list->tail;

    if (list->tail != NULL) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    list->tail = new_node;
    list->size++;
    return 0;
}

int CDLinkedList_remove(CDLinkedList *list, size_t node) {
    if (list == NULL)
        return LINKED_LIST_NULL_LIST;
    if (node >= list->size)
        return LINKED_LIST_OUT_OF_BOUNDS; 
    struct CDNode* n;
    size_t i;
    if (list->size / 2 > node) {
        n = list->head;
        i = 0;
        while (i < node) {
            n = n->next;
            i++;
        }
        n->next->prev = n->prev;
    } else {
        n = list->tail;
        i = list->size - 1;
        while (i > node) {
            n = n->prev;
            i--;
        }
        n->prev->next = n->next;
    }

    list->size--;
    free(n);
    return 0;
}

size_t CDLinkedList_find(CDLinkedList *list, void *key, CompareTo cmp) {
    if (list == NULL || cmp == NULL)
        return LINKED_LIST_NULL_LIST;

    struct CDNode *node = list->head;
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

CResult *CDLinkedList_get(const CDLinkedList *list, size_t index) {
    if (list == NULL)
        return CResult_ecreate(CError_create("Got a pointer to NULL as list.",
                                             "CDLinkedList_get",
                                             LINKED_LIST_NULL_LIST));
    if (index >= list->size)
        return CResult_ecreate(
            CError_create("Got an index exceeding the size of the list.",
                          "CDLinkedList_get", LINKED_LIST_OUT_OF_BOUNDS));

    struct CDNode *current = list->head;
    size_t mid = list->size / 2;
    if (index <= mid)
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
    else
        for (size_t i = list->size - 1; i > index; i--) {
            current = current->prev;
        }
    return current->data;
}

int CDLinkedList_set(CDLinkedList *list, size_t index, void *data) {
    if (list == NULL)
        return LINKED_LIST_NULL_LIST;
    if (index >= list->size)
        return LINKED_LIST_OUT_OF_BOUNDS;

    struct CDNode *current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    current->data = data;
    return 0;
}

void CDLinkedList_clear(CDLinkedList *list) {
    if (list == NULL)
        return;

    struct CDNode *current = list->head;
    while (current != NULL) {
        struct CDNode *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void CDLinkedList_free(CDLinkedList *list) {
    if (list == NULL)
        return;
    CDLinkedList_clear(list);
    free(list);
}
