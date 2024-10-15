#include <logger/CLog.h>
#include <util/CResult.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define ERR_CODE 1 // Error code.
#define MSG "This is some error msg."
#define CTX "This is some function context."

static void print_error(const CError *error) {
    if (error == NULL) {
        printf("Invalid error. Got pointer to NULL as error.");
        return;
    }

    CLog(ERROR, "Message: \"%s\"", CError_get_message(error));
    CLog(ERROR, "Context: \"%s\"", CError_get_context(error));
    CLog(ERROR, "Error Code: %lu", CError_get_code(error));
}

int main() {
    enable_debugging();
    CError *err = CError_create(MSG, CTX, ERR_CODE);
    assert(strcmp(CError_get_message(err), MSG) == 0);
    assert(strcmp(CError_get_context(err), CTX) == 0);
    assert(CError_get_code(err) == ERR_CODE);
    print_error(err);
    CError_free(&err);
    return 0;
}