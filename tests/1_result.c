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

int test_ok() {
    int v = 5;
    CResult_t *result = CResult_create(&v, NULL);
    assert(!CResult_is_error(result));
    assert((*(int *)CResult_get(result)) == 5);
    CResult_free(&result);
    return 0;
}

int test_error() {
    CResult_t *result = CResult_ecreate(CError_create(MSG, CTX, ERR_CODE));
    assert(CResult_is_error(result));
    CError_t *err = CResult_eget(result);
    assert(strcmp(CError_get_message(err), MSG) == 0);
    assert(strcmp(CError_get_context(err), CTX) == 0);
    assert(CError_get_code(err) == ERR_CODE);
    CResult_free(&result); // No need to call free for CError as it is managed
                           // by CResult.
    return 0;
}

int test_example() {
    // assume <assert.h> is included.
    // Create the pointer
    int a = 5;
    void *ptr = &a;
    // Create the CResult object
    CResult_t *res = CResult_create(ptr, NULL);
    // If you wanted to create an error, you would be using `CResult_ecreate`
    // instead.

    if (!CResult_is_error(res)) { // Check if the result is an error or invalid.
        // To get the CError structure/object, use `CResult_eget`.
        assert(*(int*)CResult_get(res) == a); // Perform some operation
    } else {
        return 1;
    }

    CResult_free(&res); // Pass a reference to the pointer.
    return 0;
}

int main() {
    enable_debugging();
    enable_location();
    shortened_location();
    assert(!test_ok());
    assert(!test_error());
    assert(!test_example());
    return 0;
}