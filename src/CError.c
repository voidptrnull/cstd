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
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <cstd/CError.h>
#include <cstd/CLog.h>
#include <cstd/CString.h>
#include <stdlib.h>
#include <string.h>

struct CError {
    const char *msg;            ///< Error message describing the issue.
    const char *ctx;            ///< Context in which the error occurred.
    unsigned long int err_code; ///< Error code representing the type or
                                ///< category of the error.
};

CError_t *CError_create(const char *msg, const char *ctx,
                        unsigned long int err_code) {
    CError_t *error = malloc(sizeof(CError_t));
    if (error == NULL)
        return NULL;
    error->msg = msg;
    error->ctx = ctx;
    error->err_code = err_code;
    return error;
}

void CError_free(CError_t **error) {
    if (error == NULL || *error == NULL)
        return;
    free(*error);
    *error = NULL;
}

const char *CError_get_message(const CError_t *error) {
    if (error == NULL)
        return NULL;
    return error->msg;
}

const char *CError_get_context(const CError_t *error) {
    if (error == NULL)
        return NULL;
    return error->ctx;
}

unsigned long int CError_get_code(const CError_t *error) {
    if (error == NULL)
        return 1 << sizeof(unsigned long int);
    return error->err_code;
}

#ifdef CUTILS_COMP_FUNCTIONS
#include <logger/CLog.h>
#include <stdio.h>

void print_error(const CError_t *error) {
    if (error == NULL) {
        printf("Invalid error. Got pointer to NULL as error.");
        return;
    }

    if (error->msg != NULL) {
        CLog(ERROR, "Message: \"%s\"", error->msg);
    }

    if (error->ctx != NULL) {
        CLog(ERROR, "Context: \"%s\"", error->ctx);
    }

    CLog(ERROR, "Error Code: %lu", error->err_code);
}
#endif
