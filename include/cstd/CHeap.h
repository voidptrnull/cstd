/*
 * MIT License
 *
 * Copyright (c) 2025 Subhadip Roy Chowdhury
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

/// \file CHeap.h
/// \brief Header file for the CHeap implementation.
///
/// This file defines a set of functions to manage a heap-based priority queue.
/// The heap allows for efficient extraction of the minimum element (for
/// min-heap) or maximum element (for max-heap). It provides functionalities
/// like creating a heap, extracting elements, resizing the heap, and freeing
/// the resources. Error handling is done using predefined macros, which
/// indicate success or different failure cases.

#ifndef CSTD_CHEAP_H
#define CSTD_CHEAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CResult.h"
#include "Operators.h"

/// \brief Error code indicating success.
/// \details This code is returned when an operation completes successfully.
#define CHEAP_SUCCESS 0

/// \brief Error code indicating that the heap is NULL.
/// \details This code is returned when a NULL heap is encountered.
#define CHEAP_NULL_HEAP 1

/// \brief Error code indicating that an element was not found.
/// \details This code is returned when an element is not found in the heap.
#define CHEAP_NOT_FOUND -1

/// \brief Error code indicating a failure during memory allocation.
/// \details This code is returned when memory allocation (malloc, realloc)
/// fails.
#define CHEAP_ALLOC_FAILURE 2

/// \struct CHeap
/// \brief Structure representing a heap-based priority queue.
/// \details The `CHeap` structure holds a dynamic array of elements and a
/// comparator function for organizing the heap.
typedef struct _CHeap CHeap_t;

/// \brief Create a new heap with the specified initial capacity and comparator.
/// \param initial_capacity The initial capacity to reserve for the heap.
/// \param cmp The comparator function to organize the heap.
/// \return Returns a pointer to the newly created heap encapsulated in CResult,
/// or an error code if creation fails.
CResult_t *CHeap_new(size_t initial_capacity, CompareTo cmp);

/// \brief Initialize an existing heap with the specified initial capacity and
/// comparator.
/// \param heap Pointer to the `CHeap` structure to initialize.
/// \param initial_capacity The initial capacity to reserve for the heap.
/// \param cmp The comparator function to organize the heap.
/// \return Returns `CHEAP_SUCCESS` on success, or an error code on failure.
int CHeap_init(CHeap_t *heap, size_t initial_capacity, CompareTo cmp);

/// \brief Get the current size of the heap.
/// \param heap Pointer to the heap.
/// \return The size of the heap (number of elements).
size_t CHeap_size(CHeap_t *heap);

/// \brief Resize the heap to the new specified capacity.
/// \param heap Pointer to the `CHeap` structure to resize.
/// \param new_capacity The new capacity to reserve for the heap.
/// \return Returns `CHEAP_SUCCESS` on success, or an error code on failure.
int CHeap_resize(CHeap_t *heap, size_t new_capacity);

/// \brief Insert an element into the heap.
/// \param heap Pointer to the `CHeap` structure where the element will be
/// inserted.
/// \param element A pointer to the element to insert into the heap. The element
/// must match the type expected by the heap's comparator.
/// \return Returns `CHEAP_SUCCESS` if the insertion is successful, or an error
/// code if the insertion fails (e.g., if the heap is full or if the element is
/// invalid).
int CHeap_insert(CHeap_t *heap, void *element);

/// \brief Extract the minimum (or maximum) element from the heap.
/// \param heap Pointer to the `CHeap` structure.
/// \return Returns a pointer to the extracted element, or `NULL` if extraction
/// fails.
CResult_t *CHeap_extract(CHeap_t *heap);

/// \brief Extract the minimum (or maximum) element from the heap and return as
/// a raw pointer.
/// \param heap Pointer to the `CHeap` structure.
/// \return Returns a pointer to the extracted element, or `NULL` if extraction
/// fails.
void *CHeap_fextract(CHeap_t *heap);

/// \brief Clear the heap by removing all elements.
/// \param heap Pointer to the `CHeap` structure.
/// \return Returns `CHEAP_SUCCESS` on success, or an error code on failure.
int CHeap_clear(CHeap_t *heap);

/// \brief Free the memory used by the heap.
/// \param heap Pointer to the pointer of the `CHeap` structure to free.
/// \return Returns `CHEAP_SUCCESS` on success, or an error code if memory
/// freeing fails.
int CHeap_free(CHeap_t **heap);

#ifdef __cplusplus
}
#endif

#endif // CSTD_CHEAP_H
