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

#include <cstd/CLog.h>
#include <cstd/CResult.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define ERR_CODE 1 // Error code.
#define MSG "This is some error msg."
#define CTX "This is some function context."

static void print_error(const CError_t *error) {
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
    enable_location();
    shortened_location();
    CError_t *err = CError_create(MSG, CTX, ERR_CODE);
    assert(strcmp(CError_get_message(err), MSG) == 0);
    assert(strcmp(CError_get_context(err), CTX) == 0);
    assert(CError_get_code(err) == ERR_CODE);
    print_error(err);
    CError_free(&err);
    return 0;
}