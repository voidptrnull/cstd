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


/// \file CQueue.h
/// \brief Header file for the CQueue implementation.
///
/// This file defines the functions for managing a queue data structure.
/// The queue is implemented using a linked list. The `CQueue_t` structure
/// maintains the elements of the queue and provides operations for adding,
/// removing, and clearing elements.
///
/// The library also includes error handling through predefined error codes
/// for common queue operations like adding, removing, and clearing elements.
/// The queue supports dynamic memory management, with a destructor function
/// to clean up the elements when the queue is freed.
///
/// \note This library is intended for use in C programs with manual memory
/// management. Ensure proper error checking when using the functions, especially
/// with dynamic memory operations such as adding and removing elements.
#ifndef CSTD_CQUEUE_H
#define CSTD_CQUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CResult.h"
#include "Operators.h"
#include <stddef.h>


/// \brief Opaque structure representing a queue.
///
/// The queue is implemented using a linked list. Each element in the queue
/// is linked to the next element. The front and rear pointers are used
/// to manage the flow of elements.
typedef struct _CQueue CQueue_t;

/// \brief Error code indicating the queue was successfully created.
#define CQUEUE_SUCCESS 0

/// \brief Error code indicating the queue pointer is null.
#define CQUEUE_NULL_QUEUE 1

/// \brief Error code indicating the queue is empty.
#define CQUEUE_EMPTY 2

/// \brief Error code indicating a memory allocation failure during queue
/// operations.
#define CQUEUE_ALLOC_FAILURE -1

/// \brief Error code indicating an element could not be added to the queue.
#define CQUEUE_ADD_FAILURE -2

/// \brief Error code indicating an element could not be removed from the queue.
#define CQUEUE_REMOVE_FAILURE -3

/// \brief Error code indicating a failure while clearing the queue.
#define CQUEUE_CLEAR_FAILURE -4

/// \brief Create a new queue and initialize it with the specified destructor.
/// \param destroy The destructor function to clean up elements in the queue,
/// or NULL if no destructor is needed.
/// \return Returns a pointer to the newly created `CQueue` structure, encapsulated
/// in a `CResult_t` for better error handling.
CResult_t *CQueue_new(Destructor destroy);

/// \brief Initialize a queue with the specified destructor.
/// \param queue Pointer to the `CQueue` structure to be initialized.
/// \param destroy The destructor function to clean up elements in the queue,
/// or NULL if no destructor is needed.
/// \return Returns `CQUEUE_SUCCESS` on success, or an error code if initialization
/// fails (e.g., memory allocation failure).
int CQueue_init(CQueue_t *queue, Destructor destroy);

/// \brief Get the size (number of elements) of the queue.
/// \param queue Pointer to the `CQueue` structure.
/// \return The number of elements currently in the queue.
size_t CQueue_size(CQueue_t *queue);

/// \brief Add an element to the rear of the queue.
/// \param queue Pointer to the `CQueue` structure.
/// \param element Pointer to the element to be added to the queue.
/// \return Returns `CQUEUE_SUCCESS` on success, or an error code if the operation
/// fails (e.g., memory allocation failure).
int CQueue_push(CQueue_t *queue, void *element);

/// \brief Remove and return the element at the front of the queue.
/// \param queue Pointer to the `CQueue` structure.
/// \return Returns a `CResult_t` encapsulating the element at the front of the
/// queue, or an error code if the queue is empty.
CResult_t *CQueue_pop(CQueue_t *queue);

/// \brief Clear all elements from the queue.
/// \param queue Pointer to the `CQueue` structure.
/// \return Returns `CQUEUE_SUCCESS` on success, or an error code if the operation
/// fails (e.g., failure to free memory).
int CQueue_clear(CQueue_t *queue);

/// \brief Free the memory used by the queue and all its elements.
/// \param queue Pointer to the pointer to the `CQueue` structure to be freed.
/// \return Returns `CQUEUE_SUCCESS` on success, or an error code if the operation
/// fails (e.g., failure to free memory).
int CQueue_free(CQueue_t **queue);

#ifdef __cplusplus
}
#endif

#endif // CSTD_CQUEUE_H