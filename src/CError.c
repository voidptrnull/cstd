#include <logger/CLog.h>
#include <stdlib.h>
#include <string.h>
#include <util/CError.h>
#include <util/CString.h>

struct CError {
    const char *msg;            ///< Error message describing the issue.
    const char *ctx;            ///< Context in which the error occurred.
    unsigned long int err_code; ///< Error code representing the type or
                                ///< category of the error.
};

CError *CError_create(const char *msg, const char *ctx,
                      unsigned long int err_code) {
    CError *error = malloc(sizeof(CError));
    if (error == NULL)
        return NULL;
    error->msg = msg;
    error->ctx = ctx;
    error->err_code = err_code;
    return error;
}

void CError_free(CError **error) {
    if (error == NULL || *error == NULL)
        return;
    free(*error);
    *error = NULL;
}

const char *CError_get_message(const CError *error) {
    if (error == NULL)
        return NULL;
    return error->msg;
}

const char *CError_get_context(const CError *error) {
    if (error == NULL)
        return NULL;
    return error->ctx;
}

unsigned long int CError_get_code(const CError *error) {
    if (error == NULL)
        return 1 << sizeof(unsigned long int);
    return error->err_code;
}

#ifdef CUTILS_COMP_FUNCTIONS
#include <logger/CLog.h>
#include <stdio.h>

void print_error(const CError *error) {
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
