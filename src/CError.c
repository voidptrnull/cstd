#include <CError.h>
#include <CString.h>
#include <stdlib.h>
#include <string.h>

CError* CError_create(const char *msg, const char *ctx, unsigned long int err_code) {
    CError *error = malloc(sizeof(CError));
    if (error == NULL)
        return NULL;

    CResult* res = CString_new();
    if (!CResult_is_error(res)) {
        error->msg = CResult_get(res);
        CString_set(error->msg, msg);
    } else {
        CResult_free(res);
        free(error);
        return NULL; 
    }
    CResult_free(res);

    res = CString_new();
    if (!CResult_is_error(res)) {
        error->ctx = CResult_get(res);
        CString_set(error->ctx, ctx);
    } else {
        CResult_free(res);
        CString_free(error->msg); 
        free(error);
        return NULL;
    }
    CResult_free(res);

    error->err_code = err_code;
    return error;
}


void CError_free(CError *error) {
    if (error == NULL)
        return;
    CString_free(error->ctx);
    CString_free(error->msg);
    free(error);
}

CString* CError_get_message(const CError *error) {
    if (error == NULL) return NULL;
    return error->msg;
}

CString* CError_get_context(const CError *error) {
    if (error == NULL) return NULL;
    return error->ctx;
}

unsigned long int CError_get_code(const CError *error) {
    if (error == NULL) return 1 << sizeof(unsigned long int);
    return error->err_code;
}

#ifdef CUTILS_COMP_FUNCTIONS
#include <stdio.h>
void print_error(const CError* error) {
    if (error == NULL) {
        printf("Invalid error. Got pointer to NULL as error.");
        return;
    }

    if (error->msg != NULL) {
        CResult* res = CString_c_str(error->msg);
        if (CResult_is_error(res)) {
            printf("Unable to load error message.");
        } else {
            char* str = (char*)CResult_get(res);
            printf("Error Message: \"%s\"", str);
            free(str);
        }
        CResult_free(res);
    }
        
    if (error->ctx != NULL) {
        CResult* res = CString_c_str(error->ctx);
        if (CResult_is_error(res)) {
            printf("Unable to load context of error.");
        } else {
            char* str = (char*)CResult_get(res);
            printf("Context: \"%s\"", str);
            free(str);
        }
        CResult_free(res);
    }

    printf("Error Code: %lu", error->err_code);
}
#endif