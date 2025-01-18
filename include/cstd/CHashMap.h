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

/// \file CHashMap.h
/// \brief Header file for the CHashMap library.
///
/// This file defines the data structures and functions for implementing a hash
/// map (also known as a hash table). The hash map allows for efficient
/// insertion, lookup, and removal of key-value pairs. The hash map uses a
/// array and hash functions to manage and access stored elements.
/// Use of open-addressing and linear probing has also been made.
///
/// The hash map's key-value pairs are managed with user-defined comparison and
/// hash functions.
///
/// \note The functions in this header are intended to be used with dynamic
/// memory allocation and require error checking to ensure successful memory
/// operations. If you do intend to use stack-allocated structures, do not
/// use `new` and `free` methods associated to that structure.
#ifndef CSTD_CHASHMAP_H
#define CSTD_CHASHMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CVector.h"
#include "Operators.h"

/// \def CHASHMAP_NULL_VAL
/// \brief Error code indicating that a value or key is NULL or is not valid.
#define CHASHMAP_NULL_VAL -3

/// \def CHASHMAP_NULL_MAP
/// \brief Error code indicating that the hash map is null or not initialized.
#define CHASHMAP_NULL_MAP -2

/// \def CHASHMAP_NOT_FOUND
/// \brief Error code indicating that a key was not found in the hash map.
#define CHASHMAP_NOT_FOUND -1

/// \def CHASHMAP_SUCCESS
/// \brief Success code indicating that an operation on the hash map completed
/// successfully.
#define CHASHMAP_SUCCESS 0

/// \def CHASHMAP_ALLOC_FAILURE
/// \brief Error code indicating that a memory allocation failed while
/// performing an operation on the hash map.
#define CHASHMAP_ALLOC_FAILURE 1

/// \def CHASHMAP_DEFAULT_CAPACITY
/// \brief Default initial capacity for the hash map.
/// \details The default capacity determines the number of buckets allocated
/// initially for the hash map.
#define CHASHMAP_DEFAULT_CAPACITY 64

/// \struct CHashMap
/// \brief Structure representing a hash map.
/// \details The hash map uses an array of vectors (`buckets`) to store
/// key-value pairs. Each bucket is a vector that can hold multiple entries. The
/// hash map uses user-defined comparison (`cmp`) and hash (`hash`) functions to
/// manage and access elements.
typedef struct _CHashMap CHashMap_t;

/// \brief Create a new hash map.
/// \details Allocates memory for a new hash map.
///
/// \return A pointer to a `CResult` object encapsulating the created hash map.
///         The result is successful if the hash map was created successfuly.
///
/// \note This method **DOES NOT** initialize the hash map. Please initialize it
///  using CHashMap_init() method.
///
/// \warning If memory allocation fails during the creation of the hash map,
///          this function will return an error, and no hash map will be
///          created.
CResult_t *CHashMap_new(size_t capacity, CompareTo cmp, Hash hash,
                        Destructor destroyKey, Destructor destroyValue);

/// \brief Initialize a hash map.
/// \details Allocates and initializes a new hash map with the specified
/// capacity. The provided comparison and
///          hash functions are used for managing the hash map's key-value
///          pairs.
/// \param map Pointer to the hash map to initialize.
/// \param capacity Initial number of buckets to allocate for the hash map.
/// \param cmp Comparison function for keys.
/// \param hash Hash function for keys.
/// \param destroyKey Destructor for freeing the keys.
/// \param destroyValue Destructor for freeing the values.
/// \return An integer value indicating the result of the initialization:
///         - `CHASHMAP_SUCCESS` if the hash map was successfully initialized,
///         - `CHASHMAP_ALLOC_FAILURE` if memory allocation failed.
///
/// \note Ensure that `map` is not NULL before calling this function. The
/// function initializes the hash map with the provided capacity, comparison,
/// and hash functions. The default capacity is 16 if `capacity` is zero.
///
/// \warning If memory allocation fails or the parameters are invalid, the
/// function will return `CHASHMAP_ALLOC_FAILURE`.
int CHashMap_init(CHashMap_t *map, size_t capacity, CompareTo cmp, Hash hash,
                  Destructor destroyKey, Destructor destroyValue);

/// \brief Insert a key-value pair into the hash map.
/// \details Adds a new key-value pair to the hash map. If the key already
/// exists, its value is updated.
/// \param map Pointer to the hash map.
/// \param key Pointer to the key to insert.
/// \param value Pointer to the value associated with the key.
/// \return An integer value indicating the result of the insertion:
///         - `CHASHMAP_SUCCESS` if the key-value pair was successfully inserted
///         or updated,
///         - `CHASHMAP_ALLOC_FAILURE` if memory allocation failed.
///
/// \note Ensure that neither `key` nor `value` is NULL before calling this
/// function. The function will update the value if the key already exists in
/// the hash map.
///
/// \warning If memory allocation fails, or `key` or `value` is NULL, the
/// function will return `CHASHMAP_ALLOC_FAILURE`.
int CHashMap_insert(CHashMap_t *map, void *key, void *value);

/// \brief Lookup a value by key in the hash map.
/// \details Retrieves the value associated with the given key from the hash
/// map.
/// \param map Pointer to the hash map.
/// \param key Pointer to the key to look up.
/// \return Pointer to the CResult object encapsulated with the value, which is
/// further associated with the key, or `NULL` if the key was not found.
///
/// \note Ensure that `key` is not NULL before calling this function. The
/// function will return `NULL` if the key
///       is not found in the hash map.
///
/// \warning If `key` is NULL, the function may return unexpected results or
/// fail to perform the lookup.
CResult_t *CHashMap_get(CHashMap_t *map, void *key);

/// \brief Remove a key-value pair from the hash map.
/// \details Removes the key-value pair associated with the given key from the
/// hash map.
/// \param map Pointer to the hash map.
/// \param key Pointer to the key to remove.
/// \return An integer value indicating the result of the removal:
///         - `CHASHMAP_SUCCESS` if the key-value pair was successfully removed,
///         - `CHASHMAP_NOT_FOUND` if the key was not found in the hash map.
///
/// \note Ensure that `key` is not NULL before calling this function. The
/// function will return `CHASHMAP_NOT_FOUND`
///       if the key is not present in the hash map.
///
/// \warning If `key` is NULL, the function will return `CHASHMAP_NOT_FOUND`
/// without performing the removal.
int CHashMap_remove(CHashMap_t *map, void *key);

/// \brief Free the resources used by the hash map.
/// \details Deallocates the memory used by the hash map and its buckets. The
/// hash map itself is invalid after
///          this operation and should not be used.
/// \param map Pointer to the hash map to free.
/// \return An integer value indicating the result of the operation:
///         - `CHASHMAP_SUCCESS` if the hash map was successfully freed,
///         - `CHASHMAP_NULL_MAP` if the hash map pointer was `NULL`.
///
/// \note Ensure that `map` is not NULL before calling this function. After
/// freeing, the hash map should not be used.
///
/// \warning If `map` is NULL, the function will return `CHASHMAP_NULL_MAP` and
/// will not perform any operation.
int CHashMap_free(CHashMap_t **map);

/// \brief Update the value associated with a key in the hash map.
/// \details Updates the value for a given key in the hash map. If the key does
/// not exist, the function returns an error.
/// \param map Pointer to the hash map.
/// \param key Pointer to the key whose value should be updated.
/// \param new_value Pointer to the new value to associate with the key.
/// \return An integer value indicating the result of the update:
///         - `CHASHMAP_SUCCESS` if the key-value pair was successfully updated,
///         - `CHASHMAP_NOT_FOUND` if the key was not found in the hash map.
///
/// \note Ensure that neither `key` nor `new_value` is NULL before calling this
/// function. The function will return
///       `CHASHMAP_NOT_FOUND` if the key does not exist.
///
/// \warning If memory allocation fails or `key` or `new_value` is NULL, the
/// function may return `CHASHMAP_NOT_FOUND`.
int CHashMap_update(CHashMap_t *map, void *key, void *new_value);

/// \brief Clear all key-value pairs from the hash map.
/// \details Removes all entries from the hash map, freeing the memory
/// associated with each key-value pair.
/// \param map Pointer to the hash map to clear.
/// \return An integer value indicating the result of the operation:
///         - `CHASHMAP_SUCCESS` if the hash map was successfully cleared,
///         - `CHASHMAP_NULL_MAP` if the hash map pointer was `NULL`.
///
/// \note Ensure that `map` is not NULL before calling this function. The
/// function will clear all entries but will
///       not free the hash map itself.
///
/// \warning If `map` is NULL, the function will return `CHASHMAP_NULL_MAP` and
/// will not perform the clearing operation.
int CHashMap_clear(CHashMap_t *map);

/// \brief Retrieve the number of key-value pairs in the hash map.
/// \param map Pointer to the hash map.
/// \return The number of key-value pairs currently stored in the hash map.
///
/// \note The function provides a count of the current number of key-value pairs
/// stored in the hash map.
///
/// \warning If `map` is NULL, the function returns 0.
size_t CHashMap_size(const CHashMap_t *map);

/// \brief Calculate the load factor of the hash map.
/// \details The load factor is defined as the ratio of the number of key-value
/// pairs to the number of buckets.
/// \param map Pointer to the hash map.
/// \return The load factor as a double value.
///
/// \note The load factor provides insight into the hash map's current capacity
/// utilization.
///
/// \warning If `map` is NULL, the function returns 0.0.
double CHashMap_load_factor(const CHashMap_t *map);

#ifdef __cplusplus
}
#endif

#endif // CSTD_CHASHMAP_H
