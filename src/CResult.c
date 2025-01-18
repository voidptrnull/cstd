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

#include <cstd/CResult.h>
#include <stdlib.h>

#define CRESULT_OK 0    ///< Indicates a successful operation.
#define CRESULT_ERROR 1 ///< Indicates an error occurred.

/// \internal
/// \struct CResult
/// \brief Structure representing the result of an operation.
/// \details The `CResult` structure encapsulates the result of an operation,
/// which can either be
///          a successful value or an error. It contains a `status` field to
///          indicate success or failure, and a union that holds either the
///          result value or an error pointer.
struct CResult {
    ///< Status of the result (`CRESULT_OK` or `CRESULT_ERROR`).
    size_t status;
    union {
        void *value;   ///< Pointer to the successful result value.
        CError_t *err; ///< Pointer to the error if the operation failed.
    };
    Destructor destroy;
};

CResult_t *CResult_create(void *value, Destructor destroy) {
    CResult_t *result = malloc(sizeof(struct CResult));
    if (result == NULL)
        return NULL;

    result->value = value;
    result->status = CRESULT_OK;
    result->destroy = destroy;
    return result;
}

CResult_t *CResult_ecreate(CError_t *err) {
    CResult_t *result = malloc(sizeof(CResult_t));
    if (result == NULL)
        return NULL;

    result->err = err;
    result->status = CRESULT_ERROR;
    return result;
}

int CResult_is_error(const CResult_t *result) {
    return result == NULL || result->status == CRESULT_ERROR;
}

void *CResult_get(const CResult_t *result) {
    if (result == NULL || result->status == CRESULT_ERROR)
        return NULL;
    return result->value;
}

CError_t *CResult_eget(const CResult_t *result) {
    if (result == NULL || result->status == CRESULT_OK)
        return NULL;
    return result->err;
}

int CResult_modify(CResult_t *result, void *value, Destructor destroy) {
    if (!result || !value) {
        return CRESULT_ERROR;
    }


    if (result->status == CRESULT_ERROR && result->err != NULL) {
        CError_free(&result->err);
    } else if (result->destroy != NULL) {
        result->destroy(result->value);
        result->value = NULL;
    }

    result->status = CRESULT_OK;
    result->value = value;
    result->destroy = destroy;
    return CRESULT_OK;
}

int CResult_emodify(CResult_t *result, CError_t *error) {
    if (!result || !error) {
        return CRESULT_ERROR;
    }

    if (result->status == CRESULT_ERROR && result->err != NULL) {
        CError_free(&result->err);
    } else if (result->destroy != NULL) {
        result->destroy(result->value);
        result->value = NULL;
    }

    result->status = CRESULT_ERROR;
    result->err = error;
    result->destroy = NULL;
    return CRESULT_OK;
}

void CResult_free(CResult_t **result) {
    if (result == NULL || *result == NULL)
        return;

    if ((*result)->status == CRESULT_ERROR && (*result)->err != NULL) {
        CError_free(&(*result)->err);
    } else if ((*result)->destroy != NULL) {
        (*result)->destroy((*result)->value);
        (*result)->value = NULL;
    }

    free(*result);
    *result = NULL;
}
