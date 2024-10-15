/// \file Operators.h
/// \brief Header file for all definitions of functions to be used or provided
/// by the user.
///
/// This file defines the functions for performing operations which are required
/// for other structures. This provides a standalone, standardized or one way
/// for the definitions.
///
/// The file, if enabled during compilation, also enables default
/// implementations of certain functions. To detect their presence,
/// simply use the `HAVE_CSTD_DEFAULTS` macro.
#ifndef CSTD_OPERATORS_H
#define CSTD_OPERATORS_H
#define HAVE_CSTD_DEFAULTS

#include <stddef.h>

/// \typedef CompareTo
/// \brief Function pointer type for comparison functions.
/// \param a Pointer to the first element to compare.
/// \param b Pointer to the second element to compare.
/// \return An integer indicating the result of the comparison.
typedef int (*CompareTo)(const void *a, const void *b);

/// \typedef Hash
/// \brief Function pointer type for hash functions.
/// \param key Pointer to the element to hash.
/// \return A `size_t` value representing the hash of the element.
typedef size_t (*Hash)(const void *key);

/// \typedef Destructor
/// \brief Function pointer type for destructor functions.
/// \details This type is used for functions that clean up memory or resources
/// associated with a pointer. \param data Pointer to the element to be
/// destroyed or cleaned up.
typedef void (*Destructor)(void *data);

/// \typedef CloneFn
/// \brief Function pointer type for clone functions.
/// \param data Pointer to the element to clone.
/// \return A pointer to the cloned element.
typedef void *(*CloneFn)(const void *data);

#ifndef CSTD_NO_DEFAULT_FUNCTION_IMPLEMENTATIONS
/// \brief Compare function for pointers.
/// \param a Pointer to the first element to compare.
/// \param b Pointer to the second element to compare.
/// \return An integer value indicating the result of the comparison.
int ccompare_pointer(const void *a, const void *b);

/// \brief Compare function for integers.
/// \param a Pointer to the first integer to compare.
/// \param b Pointer to the second integer to compare.
/// \return An integer value indicating the result of the comparison.
int ccompare_integer(const void *a, const void *b);

/// \brief Hash function for strings.
/// \param key Pointer to the string to hash.
/// \return A `size_t` value representing the hash of the string.
size_t chash_string(const void *key);

/// \brief Default hash function for void pointers.
/// \param key Pointer to the element to hash.
/// \return A `size_t` value representing the hash of the pointer.
size_t cdefault_hash(const void *key);

/// \brief Default clone function for integers.
/// \param data Pointer to the integer to clone.
/// \return A pointer to the cloned integer.
void *cclone_integer(const void *data);

#endif // CSTD_NO_DEFAULT_FUNCTION_IMPLEMENTATIONS

#endif // CSTD_OPERATORS_H
