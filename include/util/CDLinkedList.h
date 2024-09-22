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

/// \file CDLinkedList.h
/// \brief Header file for the CString implementation.
///
/// This file defines the functions for managing and manipulating a data
/// structure aimed to form a common implementation of a doubly-linked list. The
/// difference between this and the singly-linked list are purely based on their
/// design difference. They can be used interchangeably on a higher level. Best
/// of efforts have been taken care for optimizing the operations for generic
/// data (that is not having any specific type of data in mind).
///
/// The header includes error handling through predefined macros for success and
/// failure cases.
#ifndef CSTD_CDLINKEDLIST_H
#define CSTD_CDLINKEDLIST_H

#include "../ops/Operators.h"
#include "CResult.h"

#ifndef LINKED_LIST_NOT_FOUND
/// \brief Indicates whether an element in the linked list was found or not.
/// \details This constant is used to signal that an element was not found in
/// the list.
#define LINKED_LIST_NOT_FOUND -4
#endif

#ifndef LINKED_LIST_NULL_LIST
/// \brief Indicates whether the list is `NULL`.
/// \details This constant is used to signal that the list reference provided is
/// `NULL`.
#define LINKED_LIST_NULL_LIST -3
#endif

#ifndef LINKED_LIST_ALLOC_FAILURE
/// \brief Indicates whether there was a failure while allocating memory.
/// \details This constant is used to signal that memory allocation for any
/// component of the list failed.
#define LINKED_LIST_ALLOC_FAILURE -2
#endif

#ifndef LINKED_LIST_OUT_OF_BOUNDS
/// \brief Indicates whether a specified index exceeds the current size of the
/// list. \details This constant is used to signal that the provided index is
/// out of the valid range for the list.
#define LINKED_LIST_OUT_OF_BOUNDS -1
#endif

#ifndef LINKED_LIST_SUCCESS
/// \brief Indicates whether a particular operation was successful or not.
/// \details This constant is used to signal that an operation completed
/// successfully.
#define LINKED_LIST_SUCCESS 0
#endif

/// \struct CDLinkedList
/// \brief Structure representing a doubly linked list.
/// \details The `CDLinkedList` structure represents the entire linked list,
/// including pointers to the head and tail nodes,
///          and the size of the list. Initially, the head and tail pointers are
///          set to `NULL`, and the size is set to 0.
typedef struct CDLinkedList CDLinkedList;

/// \brief Create a new doubly linked list.
/// \return A pointer to a dynamically allocated `CResult` containing a pointer
/// to the newly created `CDLinkedList`.
CResult *CDLinkedList_new();

/// \brief Initialize a given doubly linked list.
/// \param list A pointer to the `CDLinkedList` to initialize.
/// \return `LINKED_LIST_NULL_LIST` (-3) if the list pointer is `NULL`.
///         `LINKED_LIST_ALLOC_FAILURE` (-2) if memory allocation fails.
///         `LINKED_LIST_SUCCESS` (0) if initialization is successful.
int CDLinkedList_init(CDLinkedList *list);

/// \brief Add a new node with the specified data to the end of the list.
/// \param list A pointer to the `CDLinkedList` to modify.
/// \param data A pointer to the data to be added to the list.
/// \return `LINKED_LIST_SUCCESS` (0) if the data was successfully added.
///         `LINKED_LIST_ALLOC_FAILURE` (-2) if memory allocation fails.
int CDLinkedList_add(CDLinkedList *list, void *data);

/// \brief Remove a specified node from the list.
/// \param list A pointer to the `CDLinkedList` to modify.
/// \param index Index for the node to be removed.
/// \return `LINKED_LIST_SUCCESS` (0) if the node was successfully removed.
///         `LINKED_LIST_NOT_FOUND` (-4) if the node was not found in the list.
int CDLinkedList_remove(CDLinkedList *list, size_t index);

/// \brief Find a node containing the specified key.
/// \param list A pointer to the `CDLinkedList` to search.
/// \param key A pointer to the key to search for.
/// \param cmp A comparison function used to compare nodes' data with the key.
/// \return The index of the node containing the key, or `LINKED_LIST_NOT_FOUND`
/// (-4) if not found.
size_t CDLinkedList_find(CDLinkedList *list, void *key, CompareTo cmp);

/// \brief Get the node at a specified index.
/// \param list A pointer to the `CDLinkedList` to access.
/// \param index The index of the node to retrieve.
/// \return A pointer to a dynamically allocated `CResult` containing the data
/// of the node at the specified index.
///         Returns `LINKED_LIST_OUT_OF_BOUNDS` (-1) if the index is out of
///         range.
CResult *CDLinkedList_get(const CDLinkedList *list, size_t index);

/// \brief Set the data of a node at a specified index.
/// \param list A pointer to the `CDLinkedList` to modify.
/// \param index The index of the node to modify.
/// \param data A pointer to the new data to set.
/// \return `LINKED_LIST_SUCCESS` (0) if the data was successfully set.
///         `LINKED_LIST_OUT_OF_BOUNDS` (-1) if the index is out of range.
int CDLinkedList_set(CDLinkedList *list, size_t index, void *data);

/// \brief Clear all nodes from the list.
/// \param list A pointer to the `CDLinkedList` to clear.
void CDLinkedList_clear(CDLinkedList *list);

/// \brief Free the memory associated with the list and its nodes.
/// \param list A pointer to the `CDLinkedList` to free.
void CDLinkedList_free(CDLinkedList *list);

#endif
