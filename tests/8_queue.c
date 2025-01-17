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
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <assert.h>
#include <cstd/CLog.h>
#include <cstd/CQueue.h>
#include <stdlib.h>
#include <string.h>

int test_queue_create() {
    CLog(INFO, "test_queue_create()");
    CResult_t *res = CQueue_new(free);
    assert(!CResult_is_error(res));
    CQueue_t *queue = CResult_get(res);

    assert(queue != NULL);

    CQueue_free(&queue);
    CResult_free(&res);
    return 0;
}

int test_queue_push_pop() {
    CLog(INFO, "test_queue_push_pop()");
    CResult_t *res = CQueue_new(free);
    assert(!CResult_is_error(res));
    CQueue_t *queue = CResult_get(res);

    int test_values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        int *num = malloc(sizeof(int));
        assert(num != NULL);
        *num = test_values[i];
        assert(CQueue_push(queue, num) == CQUEUE_SUCCESS);
    }

    for (int i = 0; i < 5; i++) {
        CResult_t *pop_res = CQueue_pop(queue);
        assert(!CResult_is_error(pop_res));
        int *value = CResult_get(pop_res);
        assert(*value == test_values[i]);
        free(value);
        CResult_free(&pop_res);
    }

    CQueue_free(&queue);
    CResult_free(&res);
    return 0;
}

int test_queue_pop_empty() {
    CLog(INFO, "test_queue_pop_empty()");
    CResult_t *res = CQueue_new(free);
    assert(!CResult_is_error(res));
    CQueue_t *queue = CResult_get(res);

    CResult_t *pop_res = CQueue_pop(queue);
    assert(CResult_is_error(pop_res));
    CError_t *err = CResult_eget(pop_res);
    assert(strcmp(CError_get_message(err), "Queue is empty.") == 0);

    CResult_free(&pop_res);
    CQueue_free(&queue);
    CResult_free(&res);
    return 0;
}

int test_queue_clear() {
    CLog(INFO, "test_queue_clear()");
    CResult_t *res = CQueue_new(free);
    assert(!CResult_is_error(res));
    CQueue_t *queue = CResult_get(res);

    int test_values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        int *num = malloc(sizeof(int));
        assert(num != NULL);
        *num = test_values[i];
        assert(CQueue_push(queue, num) == CQUEUE_SUCCESS);
    }

    assert(CQueue_clear(queue) == CQUEUE_SUCCESS);

    CResult_t *pop_res = CQueue_pop(queue);
    assert(CResult_is_error(pop_res));
    CError_t *err = CResult_eget(pop_res);
    assert(strcmp(CError_get_message(err), "Queue is empty.") == 0);

    CQueue_free(&queue);
    CResult_free(&res);
    return 0;
}

int test_queue_free() {
    CLog(INFO, "test_queue_free()");
    CResult_t *res = CQueue_new(free);
    assert(!CResult_is_error(res));
    CQueue_t *queue = CResult_get(res);

    CQueue_free(&queue);
    assert(queue == NULL);

    CResult_free(&res);
    return 0;
}

int main() {
    enable_location();
    shortened_location();

    assert(!test_queue_create());
    assert(!test_queue_push_pop());
    assert(!test_queue_pop_empty());
    assert(!test_queue_clear());
    assert(!test_queue_free());

    return 0;
}
