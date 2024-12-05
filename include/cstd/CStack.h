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
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/// \file CStack.h
/// \brief Header file for the CStack implementation.
///
/// This file defines the functions for managing and manipulating a stack.
/// The stack is represented by the `CStack` structure. The provided functions
/// allow for pushing, popping, and clearing elements, as well as freeing
/// the resources used by the stack.
///
/// The header includes error handling through predefined macros for success
/// and failure cases.
///
/// \note The functions in this header are intended to be used with dynamic
/// memory allocation and require error checking to ensure successful memory
/// operations. If you do intend to use stack-allocated structures, do not
/// use `new` and `free` methods associated to that structure.
#ifndef CSTD_CSTACK_H
#define CSTD_CSTACK_H

#include "CResult.h"

// Error codes for stack operations
#define CSTACK_NULL_STACK -3    ///< Stack pointer is NULL
#define CSTACK_OVERFLOW -2      ///< Stack overflow error
#define CSTACK_ALLOC_FAILURE -1 ///< Memory allocation failure
#define CSTACK_SUCCESS 0        ///< Operation successful

/// \struct CStack
/// \brief Structure for representing the stack.
///
/// The CStack structure implements a dynamic stack for storing elements.
/// It supports standard stack operations such as push, pop, and clearing the
/// stack.
typedef struct _CStack CStack_t;

/// \brief Creates a new stack.
///
/// Allocates memory for a new stack and initializes it.
/// The stack is ready for use after calling this function.
///
/// \param destroy The destructor intended to free up the pushed values.
/// \return A pointer to a CResult structure containing the stack or an error.
CResult_t *CStack_new(Destructor destroy);

/// \brief Initializes a stack.
///
/// Initializes the given stack. This function must be called before any
/// stack operations are performed on the stack. It should be called on
/// a stack that has already been allocated memory.
///
/// \param stack A pointer to the stack to be initialized.
/// \param destroy The destructor intended to free up the pushed values.
/// \return CSTACK_SUCCESS on success, or an error code on failure.
int CStack_init(CStack_t *stack, Destructor destroy);

/// \brief Pushes an item onto the stack.
///
/// Adds a new item to the top of the stack. If the stack is full,
/// it returns an overflow error.
///
/// \param stack A pointer to the stack.
/// \param item A pointer to the item to be pushed onto the stack.
/// \return CSTACK_SUCCESS on success, or an error code on failure.
int CStack_push(CStack_t *stack, void *item);

/// \brief Pops an item from the stack.
///
/// Removes and returns the item at the top of the stack. If the stack
/// is empty, it returns a NULL result indicating the stack is empty.
///
/// \param stack A pointer to the stack.
/// \return A pointer to a CResult structure containing the popped item
///         or an error.
CResult_t *CStack_pop(CStack_t *stack);

/// \brief Clears the stack.
///
/// Removes all items from the stack, freeing any associated memory.
/// The stack remains initialized and can be used again.
///
/// \param stack A pointer to the stack.
/// \return CSTACK_SUCCESS on success, or an error code on failure.
int CStack_clear(CStack_t *stack);

/// \brief Frees the stack.
///
/// Deallocates the memory used by the stack and sets the stack pointer to NULL.
///
/// \param stack A pointer to a pointer to the stack to be freed.
/// \return CSTACK_SUCCESS on success, or an error code on failure.
int CStack_free(CStack_t **stack);

#endif // CSTD_CSTACK_H
