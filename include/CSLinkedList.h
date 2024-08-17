#ifndef C_UTILS_CSLINKEDLIST_H
#define C_UTILS_CSLINKEDLIST_H

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

struct CSNode {
    void* data;
    struct CSNode* next;
};

typedef struct CSLinkedList {
    struct CSNode* node;
    size_t size;
} CSLinkedList; 

CResult* CSLinkedList_new();
int CSLinkedList_init(CSLinkedList* list);
int CSLinkedList_add(CSLinkedList* list, void* data);
int CSLinkedList_remove(CSLinkedList* list, size_t index);
size_t CSLinkedList_find(CSLinkedList* list, void* key, CompareTo cmp);
CResult* CSLinkedList_get(const CSLinkedList* list, size_t index);
int CSLinkedList_set(CSLinkedList* list, size_t index, void* data);
void CSLinkedList_clear(CSLinkedList* list);
void CSLinkedList_free(CSLinkedList* list);

#endif