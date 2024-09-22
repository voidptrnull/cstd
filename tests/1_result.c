#include <debug/debug.h>
#include <util/CResult.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define ERR_CODE 1 // Error code.
#define MSG "This is some error msg."
#define CTX "This is some function context."

int test_ok() {
    int v = 5;
    CResult *result = CResult_create(&v);
    assert(!CResult_is_error(result));
    assert((*(int *)CResult_get(result)) == v);
    CResult_free(&result);
    return 0;
}

int test_error() {
    CResult *result = CResult_ecreate(CError_create(MSG, CTX, ERR_CODE));
    assert(CResult_is_error(result));
    CError *err = CResult_eget(result);
    assert(strcmp(CError_get_message(err), MSG) == 0);
    assert(strcmp(CError_get_context(err), CTX) == 0);
    assert(CError_get_code(err) == ERR_CODE);
    CResult_free(&result); // No need to call free for cerror as it is managed
                           // by CResult.
    return 0;
}

int main() {
    assert(!test_ok());
    assert(!test_error());
    return 0;
}