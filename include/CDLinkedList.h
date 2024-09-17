#ifndef C_UTILS_CDLINKEDLIST_H
#define C_UTILS_CDLINKEDLIST_H

#include <Operators.h>
#include <CResult.h>

#ifndef LINKED_LIST_NOT_FOUND
    /// \brief Indicates whether an element in the linked list was found or not.
    /// \details This constant is used to signal that an element was not found in the list.
    #define LINKED_LIST_NOT_FOUND -4
#endif

#ifndef LINKED_LIST_NULL_LIST
    /// \brief Indicates whether the list is `NULL`.
    /// \details This constant is used to signal that the list reference provided is `NULL`.
    #define LINKED_LIST_NULL_LIST -3
#endif

#ifndef LINKED_LIST_ALLOC_FAILURE
    /// \brief Indicates whether there was a failure while allocating memory.
    /// \details This constant is used to signal that memory allocation for any component of the list failed.
    #define LINKED_LIST_ALLOC_FAILURE -2
#endif

#ifndef LINKED_LIST_OUT_OF_BOUNDS
    /// \brief Indicates whether a specified index exceeds the current size of the list.
    /// \details This constant is used to signal that the provided index is out of the valid range for the list.
    #define LINKED_LIST_OUT_OF_BOUNDS -1
#endif

#ifndef LINKED_LIST_SUCCESS
    /// \brief Indicates whether a particular operation was successful or not.
    /// \details This constant is used to signal that an operation completed successfully.
    #define LINKED_LIST_SUCCESS 0
#endif

/// \struct CDNode
/// \brief Structure for storing data in the linked list.
/// \details The `CDNode` structure is used to represent each node in the doubly linked list.
///          It contains a pointer to the data, and pointers to the previous and next nodes.
///          Initially, the previous and next pointers are set to `NULL`.
struct CDNode {
    void* data; ///< Pointer to the data stored in the node.
    struct CDNode* prev; ///< Pointer to the previous node in the list.
    struct CDNode* next; ///< Pointer to the next node in the list.
};

/// \struct CDLinkedList 
/// \brief Structure representing a doubly linked list.
/// \details The `CDLinkedList` structure represents the entire linked list, including pointers to the head and tail nodes,
///          and the size of the list. Initially, the head and tail pointers are set to `NULL`, and the size is set to 0.
typedef struct CDLinkedList {
    struct CDNode* head; ///< Pointer to the first node in the list.
    struct CDNode* tail; ///< Pointer to the last node in the list.
    size_t size; ///< The number of nodes in the list.
} CDLinkedList; 

/// \brief Create a new doubly linked list.
/// \return A pointer to a dynamically allocated `CResult` containing a pointer to the newly created `CDLinkedList`.
CResult* CDLinkedList_new();

/// \brief Initialize a given doubly linked list.
/// \param list A pointer to the `CDLinkedList` to initialize.
/// \return `LINKED_LIST_NULL_LIST` (-3) if the list pointer is `NULL`. 
///         `LINKED_LIST_ALLOC_FAILURE` (-2) if memory allocation fails.
///         `LINKED_LIST_SUCCESS` (0) if initialization is successful.
int CDLinkedList_init(CDLinkedList* list);

/// \brief Add a new node with the specified data to the end of the list.
/// \param list A pointer to the `CDLinkedList` to modify.
/// \param data A pointer to the data to be added to the list.
/// \return `LINKED_LIST_SUCCESS` (0) if the data was successfully added.
///         `LINKED_LIST_ALLOC_FAILURE` (-2) if memory allocation fails.
int CDLinkedList_add(CDLinkedList* list, void* data);

/// \brief Remove a specified node from the list.
/// \param list A pointer to the `CDLinkedList` to modify.
/// \param node A pointer to the `CDNode` to be removed.
/// \return `LINKED_LIST_SUCCESS` (0) if the node was successfully removed.
///         `LINKED_LIST_NOT_FOUND` (-4) if the node was not found in the list.
int CDLinkedList_remove(CDLinkedList* list, struct CDNode* node);

/// \brief Find a node containing the specified key.
/// \param list A pointer to the `CDLinkedList` to search.
/// \param key A pointer to the key to search for.
/// \param cmp A comparison function used to compare nodes' data with the key.
/// \return The index of the node containing the key, or `LINKED_LIST_NOT_FOUND` (-4) if not found.
size_t CDLinkedList_find(CDLinkedList* list, void* key, CompareTo cmp);

/// \brief Get the node at a specified index.
/// \param list A pointer to the `CDLinkedList` to access.
/// \param index The index of the node to retrieve.
/// \return A pointer to a dynamically allocated `CResult` containing the data of the node at the specified index.
///         Returns `LINKED_LIST_OUT_OF_BOUNDS` (-1) if the index is out of range.
CResult* CDLinkedList_get(const CDLinkedList* list, size_t index);

/// \brief Set the data of a node at a specified index.
/// \param list A pointer to the `CDLinkedList` to modify.
/// \param index The index of the node to modify.
/// \param data A pointer to the new data to set.
/// \return `LINKED_LIST_SUCCESS` (0) if the data was successfully set.
///         `LINKED_LIST_OUT_OF_BOUNDS` (-1) if the index is out of range.
int CDLinkedList_set(CDLinkedList* list, size_t index, void* data);

/// \brief Clear all nodes from the list.
/// \param list A pointer to the `CDLinkedList` to clear.
void CDLinkedList_clear(CDLinkedList* list);

/// \brief Free the memory associated with the list and its nodes.
/// \param list A pointer to the `CDLinkedList` to free.
void CDLinkedList_free(CDLinkedList* list);

#endif