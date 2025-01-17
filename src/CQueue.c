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
#include <cstd/CLinkedList.h>
#include <cstd/CQueue.h>
#include <stdlib.h>

struct _CQueue {
    CLinkedList_t *list;
    Destructor destroy;
};

CResult_t *CQueue_new(Destructor destroy) {

    CQueue_t *queue = malloc(sizeof(CQueue_t));
    if (!queue) {
        return CResult_ecreate(
            CError_create("Unable to allocate memory for the queue.",
                          "CQueue_new", CQUEUE_ALLOC_FAILURE));
    }

    CResult_t *res = CLinkedList_new(CLINKEDLIST_TYPE_SINGLE, destroy);
    if (CResult_is_error(res)) {
        free(queue);
        CError_t *err = CResult_eget(res);
        CError_modify(err, "Unable to allocate memory for elements.",
                      "CQueue_new", CQUEUE_ALLOC_FAILURE);
        return res;
    }

    queue->list = CResult_get(res);
    queue->destroy = destroy;

    CResult_modify(res, queue, NULL);
    return res;
}

int CQueue_init(CQueue_t *queue, Destructor destroy) {
    if (!queue) {
        return CQUEUE_NULL_QUEUE;
    }

    CResult_t *res = CLinkedList_new(CLINKEDLIST_TYPE_SINGLE, destroy);
    if (CResult_is_error(res)) {
        return CQUEUE_ALLOC_FAILURE;
    }

    queue->list = CResult_get(res);
    queue->destroy = destroy;

    return CQUEUE_SUCCESS;
}

size_t CQueue_size(CQueue_t *queue) {
    if (!queue) return 0;
    return CLinkedList_size(queue->list);
}

int CQueue_push(CQueue_t *queue, void *element) {
    if (!queue) {
        return CQUEUE_NULL_QUEUE;
    }

    int result = CLinkedList_add(queue->list, element);
    if (result != CLINKEDLIST_SUCCESS) {
        return CQUEUE_ADD_FAILURE;
    }

    return CQUEUE_SUCCESS;
}

CResult_t *CQueue_pop(CQueue_t *queue) {
    if (!queue) {
        return CResult_ecreate(
            CError_create("Queue is NULL.", "CQueue_pop", CQUEUE_NULL_QUEUE));
    }

    if (CLinkedList_size(queue->list) == 0) {
        return CResult_ecreate(
            CError_create("Queue is empty.", "CQueue_pop", CQUEUE_EMPTY));
    }

    CResult_t *res = CLinkedList_get(queue->list, 0);
    if (CResult_is_error(res)) {
        return res;
    }

    void *value = CResult_get(res);
    CResult_free(&res);

    int remove_result = CLinkedList_remove(queue->list, 0);
    if (remove_result != CLINKEDLIST_SUCCESS) {
        return CResult_ecreate(CError_create(
            "Failed to remove element.", "CQueue_pop", CQUEUE_REMOVE_FAILURE));
    }

    return CResult_create(value, NULL);
}

int CQueue_clear(CQueue_t *queue) {
    if (!queue) {
        return CQUEUE_NULL_QUEUE;
    }

    int result = CLinkedList_clear(queue->list);
    if (result != CLINKEDLIST_SUCCESS) {
        return CQUEUE_CLEAR_FAILURE;
    }

    return CQUEUE_SUCCESS;
}

int CQueue_free(CQueue_t **queue) {
    if (!queue || !*queue) {
        return CQUEUE_NULL_QUEUE;
    }

    if ((*queue)->list) {
        CLinkedList_free(&(*queue)->list);
    }

    free(*queue);
    *queue = NULL;

    return CQUEUE_SUCCESS;
}