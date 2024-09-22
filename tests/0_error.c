#include <debug/debug.h>
#include <util/CResult.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define ERR_CODE 1 // Error code.
#define MSG "This is some error msg."
#define CTX "This is some function context."

int main() {
    CError *err = CError_create(MSG, CTX, ERR_CODE);
    assert(strcmp(CError_get_message(err), MSG) == 0);
    assert(strcmp(CError_get_context(err), CTX) == 0);
    assert(CError_get_code(err) == ERR_CODE);
    CError_free(&err);
    return 0;
}