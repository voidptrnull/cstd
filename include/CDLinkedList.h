#ifndef C_UTILS_CDLINKEDLIST_H
#define C_UTILS_CDLINKEDLIST_H

#include <Operators.h>
#include <CResult.h>

#ifndef LINKED_LIST_NOT_FOUND
    #define LINKED_LIST_NOT_FOUND -4
#endif

#ifndef LINKED_LIST_NULL_LIST
    #define LINKED_LIST_NULL_LIST -3
#endif

#ifndef LINKED_LIST_ALLOC_FAILURE
    #define LINKED_LIST_ALLOC_FAILURE -2
#endif

#ifndef LINKED_LIST_OUT_OF_BOUNDS
    #define LINKED_LIST_OUT_OF_BOUNDS -1
#endif

#ifndef LINKED_LIST_SUCCESS
    #define LINKED_LIST_SUCCESS 0
#endif

struct CDNode {
    void* data;
    struct CDNode* prev;
    struct CDNode* next;
};

typedef struct CDLinkedList {
    struct CDNode* head;
    struct CDNode* tail;
    size_t size;
} CDLinkedList; 

CResult* CDLinkedList_new();
int CDLinkedList_init(CDLinkedList* list);
int CDLinkedList_add(CDLinkedList* list, void* data);
int CDLinkedList_remove(CDLinkedList* list, struct CDNode* node);
size_t CDLinkedList_find(CDLinkedList* list, void* key, CompareTo cmp);
CResult* CDLinkedList_get(const CDLinkedList* list, size_t index);
int CDLinkedList_set(CDLinkedList* list, size_t index, void* data);
void CDLinkedList_clear(CDLinkedList* list);
void CDLinkedList_free(CDLinkedList* list);

#endif