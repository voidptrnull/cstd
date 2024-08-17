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

/// \file operators.h
/// \brief Header file for defining function pointer types used in the CUtils library.
///
/// This file provides definitions for function pointer types that are used for comparison and hashing operations.
/// These function pointers are used by various data structures and algorithms within the library.
///
/// You, as an user, would likely want to define these for your particular use cases.
/// The file also includes default implementations for comparison and hash functions, provided that
/// the `CUTILS_COMP_FUNCTIONS` macro is defined.
/// 
/// Do note that this is not the comprehenensive or exhaustive list of definition.
/// For example, take the ErrorHandler function pointer of CError, which is defined in \file CError.h 

#ifndef C_UTILS_OPERATORS_H
#define C_UTILS_OPERATORS_H

#include <stddef.h>

/// \typedef CompareTo
/// \brief Function pointer type for comparison functions.
/// \details This type is used for functions that compare two elements. The comparison function should
///          return a negative value if the first element is less than the second, zero if they are equal,
///          and a positive value if the first element is greater than the second.
/// \param a Pointer to the first element to compare.
/// \param b Pointer to the second element to compare.
/// \return An integer value indicating the result of the comparison:
///         - Negative if the first element is less than the second,
///         - Zero if they are equal,
///         - Positive if the first element is greater than the second.
typedef int(*CompareTo)(const void* a, const void* b);

/// \typedef Hash
/// \brief Function pointer type for hash functions.
/// \details This type is used for functions that compute a hash value for a given element. The hash function
///          should return a `size_t` value representing the hash of the given element.
/// \param key Pointer to the element to hash.
/// \return A `size_t` value representing the hash of the element.
typedef size_t(*Hash)(const void* key);

#ifdef CUTILS_COMP_FUNCTIONS
/// \brief Compare function for pointers.
/// \details Compares two pointers. This function is useful for comparing memory addresses or pointers
///          to the same type of data. The comparison is based on the memory address values.
/// \param a Pointer to the first element to compare.
/// \param b Pointer to the second element to compare.
/// \return An integer value indicating the result of the comparison:
///         - Negative if the address pointed to by `a` is less than the address pointed to by `b`,
///         - Zero if the addresses are equal,
///         - Positive if the address pointed to by `a` is greater than the address pointed to by `b`.
int ccompare_pointer(const void* a, const void* b);

/// \brief Compare function for integers.
/// \details Compares two integers. This function is used to compare integer values pointed to by the
///          given pointers. The integers are dereferenced and compared directly.
/// \param a Pointer to the first integer to compare.
/// \param b Pointer to the second integer to compare.
/// \return An integer value indicating the result of the comparison:
///         - Negative if the integer pointed to by `a` is less than the integer pointed to by `b`,
///         - Zero if the integers are equal,
///         - Positive if the integer pointed to by `a` is greater than the integer pointed to by `b`.
int ccompare_integer(const void* a, const void* b);

/// \brief Hash function for strings.
/// \details Computes a hash value for a string. This function is used to generate a hash code for string
///          elements, which can be used in hash-based data structures such as hash tables. The string is
///          assumed to be null-terminated.
/// \param key Pointer to the string to hash.
/// \return A `size_t` value representing the hash of the string.
size_t chash_string(const void* key);

/// \brief Default hash function for void pointers.
/// \details Computes a hash value based on the memory address of the pointer. This function is useful for
///          hashing pointers directly, without any additional data processing. The function applies bitwise
///          operations to distribute hash values more evenly.
/// \param key Pointer to the element to hash.
/// \return A `size_t` value representing the hash of the pointer.
size_t cdefault_hash(const void* key);

#endif // CUTILS_COMP_FUNCTIONS

#endif // C_UTILS_OPERATORS_H
