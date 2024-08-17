#include <stdlib.h>
#include <CResult.h>

CResult* CResult_create(void* value) {
    CResult* result = malloc(sizeof(CResult));
    if (result == NULL)
        return NULL;

    result->value = value;  // Take ownership of value
    result->status = CRESULT_OK;
    return result;
}

CResult* CResult_ecreate(CError* err) {
    CResult* result = malloc(sizeof(CResult));
    if (result == NULL)
        return NULL;

    result->err = err;  // Take ownership of err
    result->status = CRESULT_ERROR;
    return result;
}

int CResult_is_error(const CResult *result) {
    if (result == NULL)
        return 1;
    return result->status == CRESULT_ERROR;
}

void* CResult_get(const CResult *result) {
    if (result == NULL || result->status == CRESULT_ERROR)
        return NULL;
    return result->value;
}

CError* CResult_eget(const CResult *result) {
    if (result == NULL || result->status == CRESULT_OK)
        return NULL;
    return result->err;
}

void CResult_free(CResult* result) {
    if (result == NULL)
        return;

    if (result->status == CRESULT_ERROR) {
        if (result->err != NULL) {
            CError_free(result->err);
        }
    } else {
        if (result->value != NULL) {
            free(result->value);
        }
    }
    free(result);
}