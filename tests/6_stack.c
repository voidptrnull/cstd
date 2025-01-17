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
#include <cstd/CStack.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_MAX 10000

void test_push(CStack_t *stack) {
    CLog(INFO, "test_push()");
    for (int i = 0; i < TEST_MAX; i++) {
        int *item = malloc(sizeof(int));
        *item = i;
        int result = CStack_push(stack, item);
        assert(result == CSTACK_SUCCESS);
    }
}

void test_pop(CStack_t *stack) {
    CLog(INFO, "test_pop()");
    for (int i = TEST_MAX - 1; i >= 0; i--) {
        CResult_t *result = CStack_pop(stack);
        assert(!CResult_is_error(result));
        int *item;
        item = (int *)CResult_get(result);
        assert(*item == i);
        free(item);
        CResult_free(&result);
    }
}

void test_pop_empty(CStack_t *stack) {
    CLog(INFO, "test_pop_empty()");
    CResult_t *result = CStack_pop(stack);
    assert(CResult_is_error(result));
    CResult_free(&result);
}

void test_clear(CStack_t *stack) {
    CLog(INFO, "test_clear()");
    int result = CStack_clear(stack);
    assert(result == CSTACK_SUCCESS);
}

void test_free(CStack_t **stack) {
    CLog(INFO, "test_free()");
    int result = CStack_free(stack);
    assert(result == CSTACK_SUCCESS);
}

int main() {
    enable_debugging();
    enable_location();
    shortened_location();

    CStack_t *stack;
    CResult_t *result = CStack_new(free);
    assert(result != NULL && !CResult_is_error(result));
    stack = CResult_get(result);
    CResult_free(&result);

    test_push(stack);
    test_pop(stack);
    test_pop_empty(stack);
    test_clear(stack);
    test_free(&stack);

    return 0;
}
