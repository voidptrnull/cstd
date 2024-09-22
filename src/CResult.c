#include <stdlib.h>
#include <util/CResult.h>

/// \internal
/// \enum CResultStatus
/// \brief Enumeration representing the status of a `CResult`.
/// \details The `CResultStatus` can either be `CRESULT_OK` if the operation was
/// successful,
///          or `CRESULT_ERROR` if an error occurred.
typedef enum {
    CRESULT_OK,   ///< Indicates a successful operation.
    CRESULT_ERROR ///< Indicates an error occurred.
} CResultStatus;

/// \internal
/// \struct CResult
/// \brief Structure representing the result of an operation.
/// \details The `CResult` structure encapsulates the result of an operation,
/// which can either be
///          a successful value or an error. It contains a `status` field to
///          indicate success or failure, and a union that holds either the
///          result value or an error pointer.
struct CResult {
    CResultStatus
        status; ///< Status of the result (`CRESULT_OK` or `CRESULT_ERROR`).
    union {
        void *value; ///< Pointer to the successful result value.
        CError *err; ///< Pointer to the error if the operation failed.
    };
};

CResult *CResult_create(void *value) {
    CResult *result = calloc(1, sizeof(struct CResult));
    if (result == NULL)
        return NULL;

    result->value = value; // Take ownership of value
    result->status = CRESULT_OK;
    return result;
}

CResult *CResult_ecreate(CError *err) {
    CResult *result = malloc(sizeof(CResult));
    if (result == NULL)
        return NULL;

    result->err = err; // Take ownership of err
    result->status = CRESULT_ERROR;
    return result;
}

int CResult_is_error(const CResult *result) {
    if (result == NULL)
        return 1;
    return result->status == CRESULT_ERROR;
}

void *CResult_get(const CResult *result) {
    if (result == NULL || result->status == CRESULT_ERROR)
        return NULL;
    return result->value;
}

CError *CResult_eget(const CResult *result) {
    if (result == NULL || result->status == CRESULT_OK)
        return NULL;
    return result->err;
}

void CResult_vfree(CResult **result) {
    if ((*result)->value != NULL) {
        free((*result)->value);
    }
}

void CResult_free(CResult **result) {
    if (result == NULL || *result == NULL)
        return;

    if ((*result)->status == CRESULT_ERROR) {
        if ((*result)->err != NULL) {
            CError_free(&(*result)->err);
        }
    } else {
        if ((*result)->value != NULL) {
            // free((*result)->value);
        }
    }

    free(*result);
    *result = NULL;
}
