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

/// \file CResult.h
/// \brief Header file for handling results and errors in the CUtils library.
///
/// This file defines the `CResult` structure and associated functions used for handling
/// operations that can return either a value or an error. The `CResult` type is designed
/// to simplify error handling by encapsulating the result or error in a single return type.
///
/// The file also includes functions for creating, checking, and retrieving values or errors
/// from a `CResult` object.
///

#ifndef C_UTILS_CRESULT_H
#define C_UTILS_CRESULT_H

#include <CError.h>

/// \enum CResultStatus
/// \brief Enumeration representing the status of a `CResult`.
/// \details The `CResultStatus` can either be `CRESULT_OK` if the operation was successful,
///          or `CRESULT_ERROR` if an error occurred.
typedef enum {
    CRESULT_OK,    ///< Indicates a successful operation.
    CRESULT_ERROR  ///< Indicates an error occurred.
} CResultStatus;

/// \struct CResult
/// \brief Structure representing the result of an operation.
/// \details The `CResult` structure encapsulates the result of an operation, which can either be
///          a successful value or an error. It contains a `status` field to indicate success or failure,
///          and a union that holds either the result value or an error pointer.
typedef struct CResult {
    CResultStatus status; ///< Status of the result (`CRESULT_OK` or `CRESULT_ERROR`).
    union {
        void *value;     ///< Pointer to the successful result value.
        CError *err;     ///< Pointer to the error if the operation failed.
    };
} CResult;

/// \brief Creates a `CResult` object representing a successful result.
/// \param value Pointer to the value to be encapsulated in the `CResult`.
/// \return Pointer to a newly allocated `CResult` object with `status` set to `CRESULT_OK` and the `value` field populated.
/// 
/// Note: It is advisable that you avoid stack-allocation and use heap-allocation instead.
CResult* CResult_create(void* value);

/// \brief Creates a `CResult` object representing an error.
/// \param err Pointer to the `CError` object representing the error.
/// \return Pointer to a newly allocated `CResult` object with `status` set to `CRESULT_ERROR` and the `err` field populated.
/// 
/// Note: It is advisable that you avoid stack-allocation and use heap-allocation instead.
CResult* CResult_ecreate(CError* err);

/// \brief Checks if a `CResult` object represents an error.
/// \param result Pointer to the `CResult` object to check.
/// \return `1` if the `result` represents an error (`CRESULT_ERROR`), `0` otherwise.
///
/// Edge case: If \code result \code is `NULL`, then `1` is returned.
int CResult_is_error(const CResult *result);

/// \brief Retrieves the value from a successful `CResult` object.
/// \param result Pointer to the `CResult` object from which to retrieve the value.
/// \return Pointer to the value encapsulated in the `CResult` if successful, or `NULL` if the `result` is an error.
void* CResult_get(const CResult *result);

/// \brief Retrieves the error from an error `CResult` object.
/// \param result Pointer to the `CResult` object from which to retrieve the error.
/// \return Pointer to the `CError` object encapsulated in the `CResult` if it represents an error, or `NULL` if the `result` is successful.
CError* CResult_eget(const CResult *result);

/// \brief Free the resources used by the `CResult` object.
/// \param result  The pointer to the `CResult` object.
void CResult_free(CResult* result);

#endif // C_UTILS_CRESULT_H