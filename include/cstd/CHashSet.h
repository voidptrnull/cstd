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

/// \file CHashSet.h
/// \brief Header file for the CHashSet implementation.
///
/// This file defines the functions for managing and manipulating a hash set.
/// The hash set is represented by the `CHashSet` structure.
/// The provided functions allow for adding, removing, retrieving elements,
/// checking for containment, and freeing the resources used by the set.
///
/// The header includes error handling through predefined macros for success and
/// failure cases.
///
/// \note The functions in this header are intended to be used with dynamic
/// memory allocation and require error checking to ensure successful memory
/// operations. If you do intend to use stack-allocated structures, do not
/// use `new` and `free` methods associated to that structure.
#ifndef CSTD_CHASHSET_H
#define CSTD_CHASHSET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CResult.h"
#include "Operators.h"

/// \brief Error code indicating that the key provided is `NULL`.
/// \details This code is returned when a function receives a `NULL` pointer
/// as the key parameter. It is returned in cases where an operation requires
/// a valid key, such as adding, removing, or checking for the existence of
/// a key in the hash set, and the key provided is `NULL`.
#define CHASHSET_NULL_KEY -4

/// \brief Error code indicating that the hash set already contains a particular
/// value or the absence of such. The actual error depends on the function
/// context.
/// For example, CHashSet_get returns this to indicate a value was not found.
/// Whereas, CHashSet_add returns this for re-entry of the same value.
/// \details This code is returned when a value that is attempted to be
/// added already exists in the set.
#define CHASHSET_NOT_FOUND -3

/// \brief Error code indicating that the hash set pointer is null.
/// \details This code is returned when a function receives a `NULL` pointer for
/// the hash set parameter. This is also returned in case two elements cannot be
/// compared (usually because the comparator is NULL).
#define CHASHSET_NULL_SET -2

/// \brief Error code indicating that an index is out of bounds or that a value
/// is not present.
/// \details This code is returned when an operation attempts to access or
/// remove an element at an invalid index.
#define CHASHSET_INDEX_OUT_OF_BOUNDS -1

/// \brief Success code for operations.
/// \details This code is returned when an operation completes successfully.
#define CHASHSET_SUCCESS 0

/// \brief Error code indicating a memory allocation failure.
/// \details This code is returned when a memory allocation (malloc or realloc)
/// fails.
#define CHASHSET_ALLOC_FAILURE 1

/// \struct CHashSet
/// \brief Structure representing a hash set of pointers.
/// \details The `CHashSet` structure stores a set of elements with no
/// duplicates, using a hash function to determine element locations. The
/// structure also includes functions for adding, removing, and searching
/// elements.
typedef struct _CHashSet CHashSet_t;

/// \brief Create a new hash set and initialize it with a specified initial
/// capacity.
/// \param capacity The initial capacity of the hash set.
/// \param hash The hash function to use for element indexing.
/// \param cmp The comparator for the elements.
/// \param destroy The destructor function to use for cleaning up elements, or
/// `NULL` if no destructor is needed.
/// \return Returns a pointer to the newly created `CHashSet` structure,
/// encapsulated in CResult for error handling.
CResult_t *CHashSet_new(size_t capacity, CompareTo cmp, Hash hash,
                        Destructor destroy);

/// \brief Initialize a hash set with a specified initial capacity.
/// \param set Pointer to the `CHashSet` structure to be initialized.
/// \param capacity The initial capacity of the hash set.
/// \param hash The hash function to use for element indexing.
/// \param cmp The comparator for the elements.
/// \param destroy The destructor function to use for cleaning up elements, or
/// `NULL` if no destructor is needed.
/// \return Returns `CHASHSET_SUCCESS` on success, or an error code if
/// initialization fails.
int CHashSet_init(CHashSet_t *set, size_t capacity, CompareTo cmp, Hash hash,
                  Destructor destroy);

/// \brief Add an element to the hash set.
/// \param set Pointer to the `CHashSet` structure.
/// \param value Pointer to the value to be added to the set.
/// \return Returns `CHASHSET_SUCCESS` on success, or an error code if the
/// operation fails (e.g., value already present).
int CHashSet_add(CHashSet_t *set, void *value);

/// \brief Remove an element from the hash set.
/// \param set Pointer to the `CHashSet` structure.
/// \param value Pointer to the value to be removed from the set.
/// \return Returns `CHASHSET_SUCCESS` on success, or an error code if the value
/// is not found.
int CHashSet_remove(CHashSet_t *set, void *value);

/// \brief Check if the hash set contains a specific value.
/// \param set Pointer to the `CHashSet` structure.
/// \param value Pointer to the value to search for.
/// \return Returns `CHASHSET_SUCCESS` if the value is found, or an error code
/// if the value is not found.
int CHashSet_contains(CHashSet_t *set, void *value);

/// \brief Retrieve an element from the hash set by its index.
/// \param set Pointer to the `CHashSet` structure.
/// \param key The index of the element to retrieve.
/// \return Returns a pointer to `CResult`, which in turn contains the element
/// at the specified index, or `NULL` if the index is invalid.
CResult_t *CHashSet_get(CHashSet_t *set, size_t key);

/// \brief Clear the hash set and release its resources.
/// \details This function releases the memory allocated for the elements in the
/// hash set but does not free the hash set itself.
/// \param set Pointer to the `CHashSet` structure.
/// \return Returns `CHASHSET_SUCCESS` on success, or an error code if the
/// operation fails.
int CHashSet_clear(CHashSet_t *set);

/// \brief Free the hash set and its resources.
/// \param set Pointer to the `CHashSet` structure.
/// \return Returns `CHASHSET_SUCCESS` on success, or an error code if the hash
/// set was not properly freed.
/// \note This function releases all memory associated with the hash set,
/// including the structure itself.
int CHashSet_free(CHashSet_t **set);

#ifdef __cplusplus
}
#endif

#endif // CSTD_CHASHSET_H