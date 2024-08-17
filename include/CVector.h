/**
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
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/// \file cvector.h
/// \brief Header file for the CVector library.
///
/// This file defines the functions for managing and manipulating a dynamic array of `void*` pointers.
/// The dynamic array is represented by the `CVector` structure. The provided functions allow
/// for adding, deleting, retrieving elements, sorting the vector, reserving capacity, and freeing
/// the resources used by the vector.
///
/// The library includes error handling through predefined macros for success and failure cases.
///
/// \note This library is intended to be used with dynamic memory allocation and may require error checking
///       to ensure successful memory operations.
#ifndef C_UTILS_CVECTOR_H
#define C_UTILS_CVECTOR_H

#include "Operators.h"
#include "CResult.h"

/// \brief Default growth rate for resizing the vector's capacity.
/// \details When resizing, the vector's capacity is multiplied by this growth rate.
///          The default value is 1.3 (30% growth).
#define CVECTOR_DEFAULT_GROWTH_RATE 1.3F

/// \brief Default initial capacity for the vector.
/// \details The default size of the vector when it is first initialized.
///          The default value is 32.
#define CVECTOR_DEFAULT_ALLOC_SIZE 32

/// \brief Error code indicating that the vector pointer is null.
/// \details This code is returned when a function receives a NULL pointer for the vector parameter.
#define CVECTOR_NULL_VECTOR -2

/// \brief Error code indicating that the index is out of bounds.
/// \details This code is returned when an operation attempts to access or delete an element at an invalid index.
#define CVECTOR_INDEX_OUT_OF_BOUNDS -1

/// \brief Success code for operations.
/// \details This code is returned when an operation completes successfully.
#define CVECTOR_SUCCESS 0

/// \brief Error code indicating a memory allocation failure.
/// \details This code is returned when a memory allocation (malloc or realloc) fails.
#define CVECTOR_ALLOC_FAILURE 1

/// \brief Error code indicating a failure during sorting.
/// \details This code is returned when the sorting operation fails, e.g., due to a NULL comparison function.
#define CVECTOR_SORT_FAILURE 2

/// \brief Structure representing a dynamic array of `void*` pointers.
/// \details The `CVector` structure maintains an array of `void*` pointers, its current size, and its capacity.
///          It supports dynamic resizing as elements are added or removed.
typedef struct CVector {
	void** data;  ///< Array to store data.
	size_t size;  ///< Number of elements in the vector.
	size_t capacity; ///< Capacity of the vector.
} CVector;

/// \brief Initialize the vector with a specified initial capacity.
/// \param vector Pointer to the `CVector` structure to be initialized.
/// \param reserve_capacity The capacity to reserve for the vector. If zero, a default capacity is used.
/// \return Returns `CVECTOR_SUCCESS` on success, or an error code if initialization fails.
int CVector_init(CVector* vector, size_t reserve_capacity);

/// \brief Create a new vector and initialize it with a specified initial capacity.
/// \param reserve_capacity The capacity to reserve for the vector. If zero, a default capacity is used.
/// \return Returns a pointer to the newly created `CVector` structure, encapsulated in CResult for better error handling.
CResult* CVector_new(size_t reserve_capacity);

/// \brief Add an element to the end of the vector.
/// \param vector Pointer to the `CVector` structure.
/// \param element Pointer to the element to be added to the vector.
/// \return Returns `CVECTOR_SUCCESS` on success, or an error code if the operation fails (e.g., memory allocation failure).
int CVector_add(CVector* vector, void* element);

/// \brief Delete an element from the vector at the specified index.
/// \param vector Pointer to the `CVector` structure.
/// \param index The index of the element to be deleted.
/// \return Returns `CVECTOR_SUCCESS` on success, or an error code if the operation fails (e.g., invalid index).
int CVector_del(CVector* vector, size_t index);

/// \brief Retrieve an element from the vector at the specified index.
/// \param vector Pointer to the `CVector` structure.
/// \param index The index of the element to retrieve.
/// \return Returns a pointer to CResult, which in turn contains the element at the specified index, or `NULL` if the index is invalid.
CResult* CVector_get(const CVector* vector, size_t index);

/// \brief Find the index of a specific element in the vector.
/// \param vector Pointer to the `CVector` structure.
/// \param key Pointer to the element to be searched for.
/// \param cmp The function pointer to compare the values with key.
/// \return Returns the index of the `key` if found, or `-1` if the element is not present.
size_t CVector_find(const CVector* vector, void* key, CompareTo cmp);

/// \brief Sort the elements of the vector using a comparison function.
/// \param vector Pointer to the `CVector` structure.
/// \param cmp Function pointer to the comparison function used for sorting. The comparison function should
///            return a negative value if the first element is less than the second, zero if they are equal,
///            and a positive value if the first element is greater than the second.
/// \return Returns `CVECTOR_SUCCESS` on success, or an error code if the sorting operation fails (e.g., NULL comparison function).
int CVector_sort(CVector* vector, CompareTo cmp);

/// \brief Clear the resources used by the vector.
/// \details This function releases the memory allocated for the elements and the vector's internal storage.
///          The `CVector` structure itself is not freed by this function, so it should be done by the caller if needed.
/// \param vector Pointer to the `CVector` structure.
/// \return Returns `CVECTOR_SUCCESS` on success, or an error code if the vector was not properly freed.
int CVector_clear(CVector* vector);

/// \brief Free the resources used by the vector.
/// \param vector Pointer to the `CVector` structure.
/// \return Returns `CVECTOR_SUCCESS` on success, or an error code if the vector was not properly freed.
///
/// This function releases the memory allocated for the elements and the vector's internal storage and the vector itself. The
/// `CVector` structure itself is also freed by this function, so it should not be done by the caller or double-free will occur.
int CVector_free(CVector* vector);

/// \brief Create a copy of the CVector object.
/// \param source Pointer to the source `CVector` structure.
/// \return Returns a pointer to a new `CVector` structure containing the copied data, or `NULL` if allocation fails.
///
/// This function creates a new `CVector` instance with the same content as the source `CVector`.
CResult* CVector_copy(const CVector* source);

/// \brief Reserve a specified capacity for the vector.
/// \param vector Pointer to the `CVector` structure.
/// \param new_capacity The new capacity to reserve for the vector.
/// \return Returns `CVECTOR_SUCCESS` on success, or an error code if the operation fails (e.g., memory allocation failure).
///
/// This function ensures that the vector has at least `new_capacity` capacity. If the current capacity is
/// insufficient, the vector is resized to accommodate the new capacity. If `new_capacity` is less than or
/// equal to the current capacity, no resizing is performed.
int CVector_reserve(CVector* vector, size_t new_capacity);

#endif // C_UTILS_CVECTOR_H
