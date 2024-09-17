/// \file chashmap.h
/// \brief Header file for the CHashMap library.
///
/// This file defines the data structures and functions for implementing a hash map (also known as a hash table).
/// The hash map allows for efficient insertion, lookup, and removal of key-value pairs. The hash map uses a
/// vector of buckets and hash functions to manage and access stored elements.
///
/// The hash map's key-value pairs are managed with user-defined comparison and hash functions.

#ifndef C_UTILS_CHASHMAP_H
#define C_UTILS_CHASHMAP_H

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
/// \brief Success code indicating that an operation on the hash map completed successfully.
#define CHASHMAP_SUCCESS 0

/// \def CHASHMAP_ALLOC_FAILURE
/// \brief Error code indicating that a memory allocation failed while performing an operation on the hash map.
#define CHASHMAP_ALLOC_FAILURE 1

/// \def CHASHMAP_DEFAULT_CAPACITY
/// \brief Default initial capacity for the hash map.
/// \details The default capacity determines the number of buckets allocated initially for the hash map.
#define CHASHMAP_DEFAULT_CAPACITY 16

/// \brief Structure representing a hash map.
/// \details The hash map uses an array of vectors (`buckets`) to store key-value pairs. Each bucket is a vector
///          that can hold multiple entries. The hash map uses user-defined comparison (`cmp`) and hash (`hash`)
///          functions to manage and access elements.
typedef struct {
    CVector* buckets;   ///< Array of buckets (vectors) to store hash map entries.
    size_t size;               ///< Number of key-value pairs currently in the hash map.
    size_t capacity;           ///< Number of buckets allocated for the hash map.
    CompareTo cmp;             ///< Comparison function for keys.
    Hash hash;                 ///< Hash function for keys.
} CHashMap;

/// \brief Initialize a hash map.
/// \details Allocates and initializes a new hash map with the specified capacity. The provided comparison and
///          hash functions are used for managing the hash map's key-value pairs.
/// \param map Pointer to the hash map to initialize.
/// \param capacity Initial number of buckets to allocate for the hash map.
/// \param cmp Comparison function for keys.
/// \param hash Hash function for keys.
/// \return An integer value indicating the result of the initialization:
///         - `CHASHMAP_SUCCESS` if the hash map was successfully initialized,
///         - `CHASHMAP_ALLOC_FAILURE` if memory allocation failed.
///
/// \note Ensure that `map` is not NULL before calling this function. The function initializes the hash map with
///       the provided capacity, comparison, and hash functions. The default capacity is 16 if `capacity` is zero.
///
/// \warning If memory allocation fails or the parameters are invalid, the function will return `CHASHMAP_ALLOC_FAILURE`.
int CHashMap_init(CHashMap* map, size_t capacity, CompareTo cmp, Hash hash);

/// \brief Insert a key-value pair into the hash map.
/// \details Adds a new key-value pair to the hash map. If the key already exists, its value is updated.
/// \param map Pointer to the hash map.
/// \param key Pointer to the key to insert.
/// \param value Pointer to the value associated with the key.
/// \return An integer value indicating the result of the insertion:
///         - `CHASHMAP_SUCCESS` if the key-value pair was successfully inserted or updated,
///         - `CHASHMAP_ALLOC_FAILURE` if memory allocation failed.
///
/// \note Ensure that neither `key` nor `value` is NULL before calling this function. The function will update
///       the value if the key already exists in the hash map.
///
/// \warning If memory allocation fails, or `key` or `value` is NULL, the function will return `CHASHMAP_ALLOC_FAILURE`.
int CHashMap_insert(CHashMap* map, void* key, void* value);

/// \brief Lookup a value by key in the hash map.
/// \details Retrieves the value associated with the given key from the hash map.
/// \param map Pointer to the hash map.
/// \param key Pointer to the key to look up.
/// \return Pointer to the value associated with the key, or `NULL` if the key was not found.
///
/// \note Ensure that `key` is not NULL before calling this function. The function will return `NULL` if the key
///       is not found in the hash map.
///
/// \warning If `key` is NULL, the function may return unexpected results or fail to perform the lookup.
void* CHashMap_lookup(CHashMap* map, void* key);

/// \brief Remove a key-value pair from the hash map.
/// \details Removes the key-value pair associated with the given key from the hash map.
/// \param map Pointer to the hash map.
/// \param key Pointer to the key to remove.
/// \return An integer value indicating the result of the removal:
///         - `CHASHMAP_SUCCESS` if the key-value pair was successfully removed,
///         - `CHASHMAP_NOT_FOUND` if the key was not found in the hash map.
///
/// \note Ensure that `key` is not NULL before calling this function. The function will return `CHASHMAP_NOT_FOUND`
///       if the key is not present in the hash map.
///
/// \warning If `key` is NULL, the function will return `CHASHMAP_NOT_FOUND` without performing the removal.
int CHashMap_remove(CHashMap* map, void* key);

/// \brief Free the resources used by the hash map.
/// \details Deallocates the memory used by the hash map and its buckets. The hash map itself is invalid after
///          this operation and should not be used.
/// \param map Pointer to the hash map to free.
/// \return An integer value indicating the result of the operation:
///         - `CHASHMAP_SUCCESS` if the hash map was successfully freed,
///         - `CHASHMAP_NULL_MAP` if the hash map pointer was `NULL`.
///
/// \note Ensure that `map` is not NULL before calling this function. After freeing, the hash map should not be used.
///
/// \warning If `map` is NULL, the function will return `CHASHMAP_NULL_MAP` and will not perform any operation.
int CHashMap_free(CHashMap* map);

/// \brief Update the value associated with a key in the hash map.
/// \details Updates the value for a given key in the hash map. If the key does not exist, the function returns an error.
/// \param map Pointer to the hash map.
/// \param key Pointer to the key whose value should be updated.
/// \param new_value Pointer to the new value to associate with the key.
/// \return An integer value indicating the result of the update:
///         - `CHASHMAP_SUCCESS` if the key-value pair was successfully updated,
///         - `CHASHMAP_NOT_FOUND` if the key was not found in the hash map.
///
/// \note Ensure that neither `key` nor `new_value` is NULL before calling this function. The function will return
///       `CHASHMAP_NOT_FOUND` if the key does not exist.
///
/// \warning If memory allocation fails or `key` or `new_value` is NULL, the function may return `CHASHMAP_NOT_FOUND`.
int CHashMap_update(CHashMap* map, const void* key, const void* new_value);

/// \brief Clear all key-value pairs from the hash map.
/// \details Removes all entries from the hash map, freeing the memory associated with each key-value pair.
/// \param map Pointer to the hash map to clear.
/// \return An integer value indicating the result of the operation:
///         - `CHASHMAP_SUCCESS` if the hash map was successfully cleared,
///         - `CHASHMAP_NULL_MAP` if the hash map pointer was `NULL`.
///
/// \note Ensure that `map` is not NULL before calling this function. The function will clear all entries but will
///       not free the hash map itself.
///
/// \warning If `map` is NULL, the function will return `CHASHMAP_NULL_MAP` and will not perform the clearing operation.
int CHashMap_clear(CHashMap* map);

/// \brief Retrieve the number of key-value pairs in the hash map.
/// \param map Pointer to the hash map.
/// \return The number of key-value pairs currently stored in the hash map.
///
/// \note The function provides a count of the current number of key-value pairs stored in the hash map.
///
/// \warning If `map` is NULL, the function returns 0.
inline size_t CHashMap_size(const CHashMap* map);

/// \brief Calculate the load factor of the hash map.
/// \details The load factor is defined as the ratio of the number of key-value pairs to the number of buckets.
/// \param map Pointer to the hash map.
/// \return The load factor as a double value.
///
/// \note The load factor provides insight into the hash map's current capacity utilization.
///
/// \warning If `map` is NULL, the function returns 0.0.
double CHashMap_load_factor(const CHashMap* map);

#endif // C_UTILS_CHASHMAP_H