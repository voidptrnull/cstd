#ifndef C_UTILS_CSLINKEDLIST_H
#define C_UTILS_CSLINKEDLIST_H

#include <Operators.h>
#include <CResult.h>

#ifndef LINKED_LIST_NOT_FOUND
    /// \brief Indicates that an element was not found in the linked list.
    #define LINKED_LIST_NOT_FOUND -4
#endif

#ifndef LINKED_LIST_NULL_LIST
    /// \brief Indicates that the list pointer is `NULL`.
    #define LINKED_LIST_NULL_LIST -3
#endif

#ifndef LINKED_LIST_ALLOC_FAILURE
    /// \brief Indicates that there was a failure while allocating memory.
    #define LINKED_LIST_ALLOC_FAILURE -2
#endif

#ifndef LINKED_LIST_OUT_OF_BOUNDS
    /// \brief Indicates that a specified index is out of bounds.
    #define LINKED_LIST_OUT_OF_BOUNDS -1
#endif

#ifndef LINKED_LIST_SUCCESS
    /// \brief Indicates that an operation was successful.
    #define LINKED_LIST_SUCCESS 0
#endif

/// \struct CSNode
/// \brief Represents a node in a singly linked list.
/// \details The `CSNode` structure contains a pointer to the data and a pointer to the next node in the list.
///          The `next` pointer is used to link nodes together in a singly linked list.
struct CSNode {
    void* data; ///< Pointer to the data stored in the node.
    struct CSNode* next; ///< Pointer to the next node in the list.
};

/// \struct CSLinkedList
/// \brief Represents a singly linked list.
/// \details The `CSLinkedList` structure represents the entire linked list, including a pointer to the head node
///          and the size of the list. Initially, the `node` pointer is set to `NULL`, and the `size` is set to 0.
typedef struct CSLinkedList {
    struct CSNode* node; ///< Pointer to the first node in the list.
    size_t size; ///< The number of nodes in the list.
} CSLinkedList; 

/// \brief Create a new singly linked list.
/// \return A pointer to a dynamically allocated `CResult` containing a pointer to the newly created `CSLinkedList`.
CResult* CSLinkedList_new();

/// \brief Initialize a given singly linked list.
/// \param list A pointer to the `CSLinkedList` to initialize.
/// \return `LINKED_LIST_NULL_LIST` (-3) if the list pointer is `NULL`. 
///         `LINKED_LIST_ALLOC_FAILURE` (-2) if memory allocation fails.
///         `LINKED_LIST_SUCCESS` (0) if initialization is successful.
int CSLinkedList_init(CSLinkedList* list);

/// \brief Add a new node with the specified data to the end of the list.
/// \param list A pointer to the `CSLinkedList` to modify.
/// \param data A pointer to the data to be added to the list.
/// \return `LINKED_LIST_SUCCESS` (0) if the data was successfully added.
///         `LINKED_LIST_ALLOC_FAILURE` (-2) if memory allocation fails.
int CSLinkedList_add(CSLinkedList* list, void* data);

/// \brief Remove a node at a specified index from the list.
/// \param list A pointer to the `CSLinkedList` to modify.
/// \param index The index of the node to be removed.
/// \return `LINKED_LIST_SUCCESS` (0) if the node was successfully removed.
///         `LINKED_LIST_OUT_OF_BOUNDS` (-1) if the index is out of range.
int CSLinkedList_remove(CSLinkedList* list, size_t index);

/// \brief Find the index of a node containing the specified key.
/// \param list A pointer to the `CSLinkedList` to search.
/// \param key A pointer to the key to search for.
/// \param cmp A comparison function used to compare nodes' data with the key.
/// \return The index of the node containing the key, or `LINKED_LIST_NOT_FOUND` (-4) if not found.
size_t CSLinkedList_find(CSLinkedList* list, void* key, CompareTo cmp);

/// \brief Get the data of the node at a specified index.
/// \param list A pointer to the `CSLinkedList` to access.
/// \param index The index of the node to retrieve.
/// \return A pointer to a dynamically allocated `CResult` containing the data of the node at the specified index.
///         Returns `LINKED_LIST_OUT_OF_BOUNDS` (-1) if the index is out of range.
CResult* CSLinkedList_get(const CSLinkedList* list, size_t index);

/// \brief Set the data of the node at a specified index.
/// \param list A pointer to the `CSLinkedList` to modify.
/// \param index The index of the node to modify.
/// \param data A pointer to the new data to set.
/// \return `LINKED_LIST_SUCCESS` (0) if the data was successfully set.
///         `LINKED_LIST_OUT_OF_BOUNDS` (-1) if the index is out of range.
int CSLinkedList_set(CSLinkedList* list, size_t index, void* data);

/// \brief Clear all nodes from the list.
/// \param list A pointer to the `CSLinkedList` to clear.
void CSLinkedList_clear(CSLinkedList* list);

/// \brief Free the memory associated with the list and its nodes.
/// \param list A pointer to the `CSLinkedList` to free.
void CSLinkedList_free(CSLinkedList* list);

#endif