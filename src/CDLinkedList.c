#include <CDLinkedList.h>
#include <stdlib.h>

CResult* CDLinkedList_new() {
    CDLinkedList* list = (CDLinkedList*)malloc(sizeof(CDLinkedList));
    if (list == NULL) {
        return CResult_ecreate(CError_create("Failed to allocate memory for CDLinkedList.", "CDLinkedList_new", -1));
    }
    int code = CDLinkedList_init(list);
    if (code) {
        free(list);
        return CResult_ecreate(CError_create("Failed to initialize CDLinkedList.", "CDLinkedList_new", code));
    }
    return CResult_create(list);
}

int CDLinkedList_init(CDLinkedList* list) {
    if (list == NULL) return -1;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return 0;
}

int CDLinkedList_add(CDLinkedList* list, void* data) {
    if (list == NULL) return -1;

    struct CDNode* new_node = (struct CDNode*)malloc(sizeof(struct CDNode));
    if (new_node == NULL) return -1;
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

int CDLinkedList_remove(CDLinkedList* list, struct CDNode* node) {
    if (list == NULL || node == NULL) return -1;

    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    free(node);
    list->size--;
    return 0;
}

size_t CDLinkedList_find(CDLinkedList* list, void* key, CompareTo cmp) {
    if (list == NULL || cmp == NULL)
        return LINKED_LIST_NULL_LIST;

    struct CDNode* node = list->head;
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

CResult* CDLinkedList_get(const CDLinkedList* list, size_t index) {
    if (list == NULL) return CResult_ecreate(CError_create("Got a pointer to NULL as list.", "CDLinkedList_get", LINKED_LIST_NULL_LIST));
    if (index >= list->size) return CResult_ecreate(CError_create("Got an index exceeding the size of the list.", "CDLinkedList_get", LINKED_LIST_OUT_OF_BOUNDS));

    struct CDNode* current = list->head;
    size_t mid = list->size / 2;
    if (index <= mid)
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
    else
        for (size_t i = list->size - 1;i > index;i--) {
            current = current->prev;
        }
    return current->data;
}

int CDLinkedList_set(CDLinkedList* list, size_t index, void* data) {
    if (list == NULL) return LINKED_LIST_NULL_LIST;
    if (index >= list->size) return LINKED_LIST_OUT_OF_BOUNDS;

    struct CDNode* current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    current->data = data;
    return 0;
}

void CDLinkedList_clear(CDLinkedList* list) {
    if (list == NULL) return;

    struct CDNode* current = list->head;
    while (current != NULL) {
        struct CDNode* next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void CDLinkedList_free(CDLinkedList* list) {
    if (list == NULL) return;
    CDLinkedList_clear(list);
    free(list);
}