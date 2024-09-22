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

/// \file CString.h
/// \brief Header file for the CString implementation.
///
/// This file defines the functions for managing and manipulating a data
/// structure aimed as an alternative to C-style strings. The string is
/// represented by the `CString` structure. The provided functions allow for
/// adding, deleting, retrieving characters, reserving capacity, and freeing the
/// resources used by the String.
///
/// The string is immutable, meaning the characters cannot be changed. To do so
/// would mean to create a new string altogether.
///
/// The header includes error handling through predefined macros for success and
/// failure cases.
///
/// \note The functions in this header are intended to be used with dynamic
/// memory allocation and requires error checking
///       to ensure successful memory operations.
#ifndef CSTD_CSTRING_H
#define CSTD_CSTRING_H

#include "CVector.h"
#include <stdint.h>

/// \brief Default initial capacity for the character vector.
/// \details The default size of the character vector when it is first
/// initialized.
///          The default value is 32.
#define CSTRING_DEFAULT_ALLOC_SIZE 32

#define CSTRING_OP_FAILURE -3

/// \brief Error code indicating that the character vector pointer is null.
/// \details This code is returned when a function receives a NULL pointer for
/// the vector parameter.
#define CSTRING_NULL_STRING -2

/// \brief Error code indicating that the index is out of bounds.
/// \details This code is returned when an operation attempts to access or
/// delete a character at an invalid index.
#define CSTRING_INDEX_OUT_OF_BOUNDS -1

/// \brief Success code for operations.
/// \details This code is returned when an operation completes successfully.
#define CSTRING_SUCCESS 0

/// \brief Error code indicating a memory allocation failure.
/// \details This code is returned when a memory allocation (malloc or realloc)
/// fails.
#define CSTRING_ALLOC_FAILURE 1

/// \struct CString
/// \brief Structure representing a string.
/// \details The `CString` structure uses a dynamic array to store characters.
/// It supports various
///          operations such as initialization, setting, retrieving, appending,
///          copying, and clearing.
typedef struct CString CString;

/// \brief Create a new CString object.
/// \return Returns a pointer to a newly created `CString` structure,
/// encapsulated within `CResult`.
///
/// \note The returned `CResult` will contain a pointer to a new `CString`
/// object if successful.
///       Ensure to free the `CResult` object when no longer needed to avoid
///       memory leaks.
///
/// \warning If the memory allocation fails, the returned `CResult` will
/// indicate an error.
CResult *CString_new();

/// \brief Initialize the CString object with a specified initial capacity.
/// \param string Pointer to the `CString` structure to be initialized.
/// \param size Initial capacity of the vector to be used.
/// \return Returns `CSTRING_SUCCESS` on success, or an error code if
/// initialization fails.
///
/// \note Ensure that `string` is not NULL before calling this function. Proper
/// initialization is crucial for further operations.
///
/// \warning If the memory allocation fails or the vector cannot be initialized,
/// an appropriate error code will be returned.
int CString_init(CString *string, size_t size);

/// \brief Get a character at a specific index in the CString object.
/// \param string Pointer to the `CString` structure.
/// \param index Index of the character to retrieve.
/// \return Returns the character at the specified index, or an error code if
/// the index is out of bounds.
///
/// \note Ensure that `string` is not NULL and the index is within bounds before
/// calling this function. The function will return an error code if the index
/// is invalid.
///
/// \warning If `index` is out of bounds, or if `string` is NULL, the function
/// may not behave as expected.
char CString_at(const CString *string, size_t index);

/// \brief Get the length of the CString object.
/// \param string Pointer to the `CString` structure.
/// \return Returns the length of the string, or an error code if the string is
/// null.
///
/// \note Ensure that `string` is not NULL before calling this function. The
/// function will return an error code if `string` is NULL.
///
/// \warning If `string` is NULL or not properly initialized, the function may
/// return an error code.
size_t CString_length(const CString *string);

/// \brief Append a string to the end of the CString object.
/// \param string Pointer to the `CString` structure.
/// \param str Pointer to the string to be appended.
/// \return Returns `CSTRING_SUCCESS` on success, or an error code if the
/// operation fails.
///
/// \note This function handles dynamic resizing of the `CString` if necessary.
/// Ensure that `str` is not NULL before calling this function.
///
/// \warning If memory allocation fails or the string cannot be appended, an
/// appropriate error code will be returned.
int CString_append_c(CString *string, const char *str);

/// \brief Append a string to the end of the CString object.
/// \param string Pointer to the `CString` structure.
/// \param str_to_add Pointer to the `CString` structure to be appended.
/// \return Returns `CSTRING_SUCCESS` on success, or an error code if the
/// operation fails.
///
/// \note This function handles dynamic resizing of the `CString` if necessary.
/// Ensure that `str_to_add` is not NULL before calling this function.
///
/// \warning If memory allocation fails or the `CString` cannot be appended, an
/// appropriate error code will be returned.
int CString_append(CString *string, CString *str_to_add);

/// \brief Check if two `CString` objects are equal.
/// \param str1 Pointer to the first `CString` object.
/// \param str2 Pointer to the second `CString` object.
/// \return Returns 1 if they are equal, 0 otherwise.
///
/// \details This function performs a lexicographical comparison of the two
/// `CString` objects. Ensure both `CString` objects are properly initialized
/// before calling this function.
///
/// \warning If either of the `CString` objects is NULL or not properly
/// initialized, the function may return unexpected results.
int CString_equals(CString *str1, CString *str2);

/// \brief Extract a substring from the CString object.
/// \param string Pointer to the `CString` structure.
/// \param start Starting index of the substring (inclusive).
/// \param end Ending index of the substring (exclusive).
/// \return Returns a `CResult` structure containing a new `CString` with the
/// substring, or an error code if the operation fails.
///
/// \details This function extracts a substring from the `CString` based on the
/// given start and end indices. The new `CString` contains characters from
/// `start` to `end - 1`.
///       The `start` index is inclusive and the `end` index is exclusive. If
///       `start` is greater than or equal to `end`, or if either index is out
///       of bounds, an error `CResult` will be returned. The caller is
///       responsible for freeing the memory associated with the `CResult` and
///       the newly created `CString` object.
///
/// \warning If `string` is NULL or the indices are out of bounds, the function
/// returns a result encapsulating an error.
CResult *CString_substring(const CString *string, size_t start, size_t end);

/// \brief Create a copy of the CString object.
/// \param source Pointer to the source `CString` structure.
/// \return Returns a pointer to a new `CResult` structure, containing a
/// `CString` with the copied data, or an error code if the operation fails.
///
/// \note The returned `CResult` will contain a pointer to a new `CString`
/// object with the same content as the source `CString`. Ensure to free the
/// `CResult` and the copied `CString` when no longer needed.
///
/// \warning If the source `CString` is NULL or memory allocation fails, the
/// function will return an error `CResult`.
CResult *CString_copy(const CString *source);

/// \brief Convert the contents of the CString object to a C-Style string.
/// \param string Pointer to the `CString` structure to be converted from.
/// \return Returns a `CResult` structure, which has the C-String as `char*` or
/// an error `CResult` in case of failure.
///
/// \note The resultant string is dynamically allocated, so it is necessary to free
/// it to avoid memory leaks. Ensure that `string` is properly initialized
/// before calling this function.
///
/// \warning If memory allocation fails or `string` is NULL, the function will
/// return an error `CResult`.
CResult *CString_c_str(CString *string);

/// \brief Set the contents of the CString object.
/// \param string Pointer to the `CString` structure.
/// \param str Pointer to the string to set.
/// \return Returns `CSTRING_SUCCESS` on success, or an error code if the
/// operation fails.
///
/// \note The function will clear the current contents of the `CString` before
/// setting the new string. Ensure that `str` is not NULL.
///
/// \warning If memory allocation fails or the provided string cannot be
/// processed, an appropriate error code will be returned.
int CString_set(CString *string, const char *str);

/// \brief Compare two CString objects lexicographically.
/// \param str1 Pointer to the first `CString` structure.
/// \param str2 Pointer to the second `CString` structure.
/// \return A signed long integer representing the result of the comparison:
///         - A negative value if `str1` is less than `str2`.
///         - Zero if `str1` is equal to `str2`.
///         - A positive value if `str1` is greater than `str2`.
///
/// \note This function compares two `CString` objects lexicographically based
/// on the character data they contain. The comparison is case-sensitive. Ensure
/// that both `CString` objects are properly initialized before calling this
/// function.
///
/// \warning If either `CString` object is NULL or contains non-standard
/// characters, the function returns `-0x8000000000000000L`.
int64_t CString_compare(CString *str1, CString *str2);

/// \brief Clear the contents of the CString object.
/// \param string Pointer to the `CString` structure to be cleared.
/// \return Returns `CSTRING_SUCCESS` on success, or an error code if the
/// clearing operation fails.
///
/// \note This function removes all characters from the `CString` while
/// retaining the allocated memory for reuse. Ensure that `string` is properly
/// initialized before calling this function.
///
/// \warning If `string` is NULL or memory allocation fails during clearing, the
/// function may return an error code.
int CString_clear(CString *string);

/// \brief Free the resources used by the CString object. This is equivalent to
/// freeing/deleting the object. \param string Pointer to the pointer to the
/// `CString` structure to be freed. \return Returns `CSTRING_SUCCESS` on
/// success, or an error code if the freeing operation fails.
///
/// \note This function releases the memory allocated for the CString's internal
/// data and the CString itself.
///
/// \warning Ensure that `string` is properly initialized before calling this
/// function. If `string` is NULL, the function will return `CSTRING_SUCCESS`.
int CString_free(CString **string);

#endif // CSTD_CSTRING_H
