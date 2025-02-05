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

/// \file CResult.h
/// \brief Header file for handling results and errors in the CUtils library.
///
/// This file defines the `CResult` structure and associated functions used for
/// handling operations that can return either a value or an error. The
/// `CResult` type is designed to simplify error handling by encapsulating the
/// result or error in a single return type.
///
/// The file also includes functions for creating, checking, and retrieving
/// values or errors from a `CResult` object.
///
/// \note Any time you encounter a library function returning a pointer to the
/// `CResult` structure, you should infer that:
///    - It had been dynamically allocated on the heap.
///    - It needs to be freed when their use is sufficed and are no longer
///    needed. <br><br>
///   To accomplish both, assume the following example code:
///   \code{.c}
///   // assume <assert.h> is included.
///   // Create the pointer
///   int a = 5;
///   void* ptr = &a;
///   // Create the CResult object
///   CResult* res = CResult_create(ptr, NULL); // If this was malloc'd
///                               // you would pass free or custom function.
///
///   // If you wanted to create an error, you would be using `CResult_ecreate`
///   // instead.
///
///   if (!CResult_is_error(res)) { // Check if the result is an error or
///   invalid.
///      // To get the CError structure/object, use `CResult_eget`.
///      assert(*(int*)CResult_get(res) == a); // Perform some operation
///   }
///   CResult_free(&res); // Pass a reference to the pointer.
///   \endcode
///
#ifndef CSTD_CRESULT_H
#define CSTD_CRESULT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CError.h"
#include "Operators.h"

/// \struct CResult
/// \brief Structure for wrapping the value or error.
typedef struct CResult CResult_t;

/// \brief Creates a `CResult` object representing a successful result.
/// \param value Pointer to the value to be encapsulated in the `CResult`.
/// \param destroy Function pointer for custom free function. This is used to
/// free the value.
/// \return Pointer to a newly allocated `CResult` object with
/// `status` set to `CRESULT_OK` and the `value` field populated.
CResult_t *CResult_create(void *value, Destructor destroy);

/// \brief Creates a `CResult` object representing an error.
/// \param err Pointer to the `CError` object representing the error.
/// \return Pointer to a newly allocated `CResult` object with `status` set to
/// `CRESULT_ERROR` and the `err` field populated.
///
/// \note It is advisable that you avoid stack-allocation and use
/// heap-allocation instead.
CResult_t *CResult_ecreate(CError_t *err);

/// \brief Checks if a `CResult` object represents an error.
/// \param result Pointer to the `CResult` object to check.
/// \return `1` if the `result` represents an error (`CRESULT_ERROR`), `0`
/// otherwise.
///
/// \note  Edge case: If  result is `NULL`, then `1` is returned.
int CResult_is_error(const CResult_t *result);

/// \brief Retrieves the value from a successful `CResult` object.
/// \param result Pointer to the `CResult` object from which to retrieve the
/// value.
/// \return Pointer to the value encapsulated in the `CResult` if successful, or
/// `NULL` if the `result` is an error.
void *CResult_get(const CResult_t *result);

/// \brief Retrieves the error from an error `CResult` object.
/// \param result Pointer to the `CResult` object from which to retrieve the
/// error.
/// \return Pointer to the `CError` object encapsulated in the `CResult`  if it
/// represents an error, or `NULL` if the `result` is successful.
CError_t *CResult_eget(const CResult_t *result);

/// \brief Modify the existing data of the CResult_t, thus effectively allowing
/// to avoid excessive memory allocations in cases where it can be avoided.
/// \param value Pointer to the value to be encapsulated in the `CResult`.
/// \param destroy Function pointer for custom free function. This is used to
/// free the value.
/// \return `0` if modification is a success, `1` otherwise.
int CResult_modify(CResult_t *result, void *value, Destructor destroy);

/// \brief Modify the existing data of the CResult_t, thus effectively allowing
/// to avoid excessive memory allocations in cases where it can be avoided. This
/// is a variation for CError_t strucutre.
/// \param error Pointer to the `CError` stricture.
/// \return `0` if modification is a success, `1` otherwise.
int CResult_emodify(CResult_t *result, CError_t *error);

/// \brief Free the resources used by the `CResult` object.
/// \param result  The pointer to the pointer to the `CResult` object.
///
/// \attention This does not free the value if the result is successful.
/// Instead, it uses the provided `destroy` function (if any) to free the value
/// encapsulated in the `CResult` when the result is successful. If the result
/// represents an error, the error object is freed via `CError_free`.
///
/// \note The `destroy` function should handle freeing the memory for the value
/// stored in the `CResult`. If `destroy` is `NULL`, no custom cleanup will be
/// done. For error results, the `err` field (if non-NULL) is freed using
/// `CError_free`.
///
/// After freeing the `CResult` object, the pointer to the `CResult` is set to
/// `NULL` to avoid dangling references.
void CResult_free(CResult_t **result);

#ifdef __cplusplus
}
#endif

#endif // CSTD_CRESULT_H
