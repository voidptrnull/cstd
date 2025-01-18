/*
 * MIT License
 *
 * Copyright (c) 2024 Subhadip Roy Chowdhury
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/// \file CLinkedList.h
/// \brief Header file for the CLinkedList implementation.
///
/// This file defines the functions for managing and manipulating a doubly
/// linked list. The linked list is represented by the `CLinkedList` structure.
/// The provided functions allow for adding, deleting, retrieving elements,
/// searching the list, and freeing the resources used by the list.
///
/// The header includes error handling through predefined macros for success
/// and failure cases.
///
/// \note The functions in this header are intended to be used with dynamic
/// memory allocation and require error checking to ensure successful memory
/// operations. If you do intend to use stack-allocated structures, do not
/// use `new` and `free` methods associated to that structure.
#ifndef CSTD_CLINKEDLIST_H
#define CSTD_CLINKEDLIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CResult.h"
#include "Operators.h"

/// \def CLINKEDLIST_TYPE_SINGLE
/// \brief Constant representing a singly linked list type.
/// \details This value should be used to specify that the linked list is a
/// singly linked list, where each node contains a pointer to the next node in
/// the list, and there is no  pointer to the previous node. This type is
/// generally more memory efficient but can be slower for certain operations,
/// such as removing elements from the end of the list. It is typically used
/// when the list is meant to only traverse in one direction.
#define CLINKEDLIST_TYPE_SINGLE 0

/// \def CLINKEDLIST_TYPE_DOUBLE
/// \brief Constant representing a doubly linked list type.
/// \details This value should be used to specify that the linked list is a
/// doubly linked list, where each node contains pointers to both the next and
/// previous nodes in the list. This type is more flexible as it allows for
/// traversal in both directions, and provides faster operations for removing
/// elements from the end of the list or in the middle. However, it requires
/// more memory as each node needs an extra pointer.
#define CLINKEDLIST_TYPE_DOUBLE 1

/// \brief Error code indicating that the linked list pointer is null.
/// \details This code is returned when a function receives a NULL pointer for
/// the list parameter.
#define CLINKEDLIST_NULL_LIST -2

/// \brief Error code indicating that the index is out of bounds.
/// \details This code is returned when an operation attempts to access or
/// delete an element at an invalid index.
#define CLINKEDLIST_INDEX_OUT_OF_BOUNDS -1

/// \brief Success code for operations.
/// \details This code is returned when an operation completes successfully.
#define CLINKEDLIST_SUCCESS 0

/// \brief Error code indicating a memory allocation failure.
/// \details This code is returned when a memory allocation (malloc or realloc)
/// fails.
#define CLINKEDLIST_ALLOC_FAILURE 1

/// \struct CLinkedList
/// \brief Structure representing a doubly linked list.
/// \details The linked list maintains pointers to the head and tail nodes,
/// and supports operations like adding, removing, and retrieving elements.
typedef struct _CLinkedList CLinkedList_t;

/// \brief Create a new linked list.
/// \param list_type Specifies the type of the list(Singly or Doubly). Use the
/// `CLINKEDLIST_TYPE_SINGLE` nad `CLINKEDLIST_TYPE_DOUBLE` macros for this.
/// \return Returns a pointer to the newly created `CLinkedList` structure,
/// encapsulated in CResult for better error handling.
CResult_t *CLinkedList_new(int list_type, Destructor destroy);

/// \brief Initialize the linked list.
/// \param list Pointer to the `CLinkedList` structure to be initialized.
/// \param list_type Specifies the type of the list(Singly or Doubly). Use the
/// `CLINKEDLIST_TYPE_SINGLE` nad `CLINKEDLIST_TYPE_DOUBLE` macros for this.
/// \return Returns `CLINKEDLIST_SUCCESS` on success, or an error code if
/// initialization fails.
int CLinkedList_init(CLinkedList_t *list, int list_type, Destructor destroy);

/// \brief Add an element to the end of the list.
/// \param list Pointer to the `CLinkedList` structure.
/// \param element Pointer to the element to be added.
/// \return Returns `CLINKEDLIST_SUCCESS` on success, or an error code if
/// the operation fails (e.g., memory allocation failure).
int CLinkedList_add(CLinkedList_t *list, void *element);

/// \brief Remove an element from the list at the specified index.
/// \param list Pointer to the `CLinkedList` structure.
/// \param index The index of the element to remove.
/// \return Returns `CLINKEDLIST_SUCCESS` on success, or an error code if
/// the operation fails (e.g., invalid index).
int CLinkedList_remove(CLinkedList_t *list, size_t index);

/// \brief Retrieve an element from the list at the specified index.
/// \param list Pointer to the `CLinkedList` structure.
/// \param index The index of the element to retrieve.
/// \return Returns a pointer to CResult, which in turn contains the element
/// at the specified index, or `NULL` if the index is invalid.
CResult_t *CLinkedList_get(const CLinkedList_t *list, size_t index);

/// \brief Find the index of a specific element in the list.
/// \param list Pointer to the `CLinkedList` structure.
/// \param key Pointer to the element to be searched for.
/// \param cmp The function pointer to compare the values with the key.
/// \return Returns the index of the `key` if found, or `-1UL` if the element
/// is not present.
size_t CLinkedList_find(const CLinkedList_t *list, void *key, CompareTo cmp);

/// \brief Clear the resources used by the list.
/// \details This function releases the memory allocated for the elements and
/// the list's internal storage.
/// \param list Pointer to the `CLinkedList` structure.
/// \return Returns `CLINKEDLIST_SUCCESS` on success, or an error code if
/// the list was not properly freed.
int CLinkedList_clear(CLinkedList_t *list);

/// \brief Free the resources used by the list.
/// \param list Pointer to the pointer to the `CLinkedList` structure.
/// \return Returns `CLINKEDLIST_SUCCESS` on success, or an error code if
/// the list was not properly freed.
/// This function releases the memory allocated for the elements and the
/// list's internal storage and the list itself. The `CLinkedList` structure
/// is also freed by this function, so it should not be done by the caller
/// or double-free will occur.
int CLinkedList_free(CLinkedList_t **list);

/// \brief Clone the linked list.
/// \param source Pointer to the source `CLinkedList` structure.
/// \param cloner Function pointer for clone function.
/// \return Returns a pointer to a new `CLinkedList` structure containing the
/// deep copied data, or `NULL` if allocation fails.
CResult_t *CLinkedList_clone(const CLinkedList_t *source, CloneFn cloner);

/// \brief Get the size of the list.
/// \param list Pointer to the `CLinkedList` structure.
/// \return The size of the list (number of elements).
size_t CLinkedList_size(const CLinkedList_t *list);

#ifdef __cplusplus
}
#endif

#endif // CSTD_CLINKEDLIST_H
