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
#ifndef CSTD_CRESULT_H
#define CSTD_CRESULT_H

#include "CError.h"

/// \struct CResult
/// \brief Structure for wrapping the value or error.
typedef struct CResult CResult;

/// \brief Creates a `CResult` object representing a successful result.
/// \param value Pointer to the value to be encapsulated in the `CResult`.
/// \return Pointer to a newly allocated `CResult` object with `status` set to
/// `CRESULT_OK` and the `value` field populated.
///
/// \note It is advisable that you avoid stack-allocation and use
/// heap-allocation instead.
CResult *CResult_create(void *value);

/// \brief Creates a `CResult` object representing an error.
/// \param err Pointer to the `CError` object representing the error.
/// \return Pointer to a newly allocated `CResult` object with `status` set to
/// `CRESULT_ERROR` and the `err` field populated.
///
/// \note It is advisable that you avoid stack-allocation and use
/// heap-allocation instead.
CResult *CResult_ecreate(CError *err);

/// \brief Checks if a `CResult` object represents an error.
/// \param result Pointer to the `CResult` object to check.
/// \return `1` if the `result` represents an error (`CRESULT_ERROR`), `0`
/// otherwise.
///
/// \note  Edge case: If  result is `NULL`, then `1` is returned.
int CResult_is_error(const CResult *result);

/// \brief Retrieves the value from a successful `CResult` object.
/// \param result Pointer to the `CResult` object from which to retrieve the
/// value. \return Pointer to the value encapsulated in the `CResult` if
/// successful, or `NULL` if the `result` is an error.
void *CResult_get(const CResult *result);

/// \brief Retrieves the error from an error `CResult` object.
/// \param result Pointer to the `CResult` object from which to retrieve the
/// error. \return Pointer to the `CError` object encapsulated in the `CResult`
/// if it represents an error, or `NULL` if the `result` is successful.
CError *CResult_eget(const CResult *result);

/// \brief Free the value used by the `CResult` object.
/// \param result The pointer to the `CResult` object.
///
/// \attention This frees the value of the result object. If it was not
/// heap-allocated (i.e. allocated on stack), it will cause a double free.
void CResult_vfree(CResult **result);

/// \brief Free the resources used by the `CResult` object.
/// \param result  The pointer to the pointer to the `CResult` object.
///
/// \attention This does not free the value if the result is successful. Use
/// CResult_vfree() instead before calling this method.
void CResult_free(CResult **result);

#endif // CSTD_CRESULT_H