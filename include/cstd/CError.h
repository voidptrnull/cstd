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

/// \file CError.h
/// \brief Header file for error handling in the CUtils library.
///
/// This file defines the `CError` structure and associated functions used for
/// representing and managing errors within the library. The `CError` type
/// provides a standardized way to report and handle errors, including error
/// messages, contexts, and error codes.
///
/// The file also includes optional error-handling utilities if the
/// `CUTILS_COMP_FUNCTIONS` macro is defined.
///

#ifndef CSTD_CERROR_H
#define CSTD_CERROR_H

#include <stddef.h>

/// \struct CError
/// \brief Structure representing an error in the CUtils library.
/// \details The `CError` structure encapsulates an error message, context, and
/// an error code.
///          It is used to provide detailed information about errors that occur
///          during the execution of various library functions.
typedef struct CError CError_t;

/// \brief Creates a new `CError` object.
/// \details Allocates and initializes a `CError` object with the provided
/// message, context, and error code. \param msg The error message as a C
/// string. \param ctx The context in which the error occurred as a C string.
/// \param err_code The error code as an unsigned long integer.
/// \return Pointer to the newly allocated `CError` object.
CError_t *CError_create(const char *msg, const char *ctx,
                      unsigned long int err_code);

/// \brief Frees a `CError` object.
/// \details Releases the memory associated with the given `CError` object,
/// including its message and context strings. \param error Pointer to the
/// pointer to the `CError` object to free.
void CError_free(CError_t **error);

/// \brief Retrieves the error message from a `CError` object.
/// \param error Pointer to the `CError` object.
/// \return Pointer to the string containing the error message.
const char *CError_get_message(const CError_t *error);

/// \brief Retrieves the context from a `CError` object.
/// \param error Pointer to the `CError` object.
/// \return Pointer to the string containing the context in which the error
/// occurred.
const char *CError_get_context(const CError_t *error);

/// \brief Retrieves the error code from a `CError` object.
/// \param error Pointer to the `CError` object.
/// \return The error code as an unsigned long integer.
unsigned long int CError_get_code(const CError_t *error);

/// \typedef ErrorHandler
/// \brief Function pointer type for error handling functions.
/// \details This type defines a function pointer for handling errors
/// represented by `CError` objects.
///          Error handling functions of this type can be used to log, display,
///          or otherwise process errors.
/// \param error Pointer to the `CError` object representing the error.
typedef void (*ErrorHandler)(const CError_t *error);

#ifdef CUTILS_COMP_FUNCTIONS
/// \brief Prints a `CError` object to standard output.
/// \details If the `CUTILS_COMP_FUNCTIONS` macro is defined, this function can
/// be used to print the details of a `CError`
///          object, including the error message, context, and error code.
/// \param error Pointer to the `CError` object to print.
void print_error(const CError_t *error);
#endif

#endif // CSTD_CERROR_H
