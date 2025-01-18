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

#include <cstd/CHeap.h>
#include <stdlib.h>

struct _CHeap {
    void **data;
    size_t size;
    size_t capacity;
    CompareTo cmp;
    Destructor destroy;
};

static inline void CHeap_heapify_up(CHeap_t *heap, size_t index);
static inline void CHeap_heapify_down(CHeap_t *heap, size_t index);

CResult_t *CHeap_new(size_t initial_capacity, Destructor destroy,
                     CompareTo cmp) {
    CHeap_t *heap = malloc(sizeof(CHeap_t));
    if (!heap)
        return CResult_ecreate(
            CError_create("Unable to allocate memory for heap.", "CHeap_new",
                          CHEAP_ALLOC_FAILURE));
    if (CHeap_init(heap, initial_capacity, destroy, cmp)) {
        free(heap);
        return CResult_ecreate(
            CError_create("Unable to allocate memory for heap data.",
                          "CHeap_new", CHEAP_ALLOC_FAILURE));
    }
    return CResult_create(heap, NULL);
}

int CHeap_init(CHeap_t *heap, size_t initial_capacity, Destructor destroy,
               CompareTo cmp) {
    if (!heap)
        return CHEAP_NULL_HEAP;
    heap->data = malloc(initial_capacity * sizeof(void *));
    if (!heap->data)
        return CHEAP_ALLOC_FAILURE;
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->destroy = destroy;
    heap->cmp = cmp;
    return CHEAP_SUCCESS;
}

size_t CHeap_size(CHeap_t *heap) {
    if (!heap)
        return __SIZE_MAX__;
    return heap->size;
}

int CHeap_resize(CHeap_t *heap, size_t new_capacity) {
    if (!heap || !heap->data)
        return CHEAP_NULL_HEAP;
    void **new_data = realloc(heap->data, new_capacity * sizeof(void *));
    if (!new_data)
        return CHEAP_ALLOC_FAILURE;
    heap->data = new_data;
    heap->capacity = new_capacity;
    return CHEAP_SUCCESS;
}

int CHeap_insert(CHeap_t *heap, void *element) {
    if (!heap || !heap->data)
        return CHEAP_NULL_HEAP;
    if (heap->size == heap->capacity) {
        if (CHeap_resize(heap, heap->capacity * 2))
            return CHEAP_NOT_FOUND;
    }
    heap->data[heap->size] = element;
    CHeap_heapify_up(heap, heap->size);
    heap->size++;
    return 0;
}

CResult_t *CHeap_extract(CHeap_t *heap) {
    if (!heap || !heap->data)
        return CResult_ecreate(
            CError_create("Heap is null.", "CHeap_extract", CHEAP_NULL_HEAP));
    ;
    if (heap->size == 0)
        return CResult_ecreate(
            CError_create("Heap is empty.", "CHeap_extract", CHEAP_NOT_FOUND));
    void *min = heap->data[0];
    heap->size--;
    heap->data[0] = heap->data[heap->size];
    CHeap_heapify_down(heap, 0);
    return CResult_create(min, NULL);
}

void *CHeap_fextract(CHeap_t *heap) {
    if (heap->size == 0)
        return NULL;
    void *min = heap->data[0];
    heap->size--;
    heap->data[0] = heap->data[heap->size];
    CHeap_heapify_down(heap, 0);
    return min;
}

int CHeap_clear(CHeap_t *heap) {
    if (!heap)
        return CHEAP_NULL_HEAP;
    if (heap->destroy)
        for (size_t i = 0; i < heap->size; i++)
            heap->destroy(heap->data[i]);
    free(heap->data);
    return CHEAP_SUCCESS;
}

int CHeap_free(CHeap_t **heap) {
    if (!CHeap_clear(*heap))
        free(*heap);
    return CHEAP_SUCCESS;
}

static inline void CHeap_heapify_up(CHeap_t *heap, size_t index) {
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (heap->cmp(heap->data[index], heap->data[parent]) >= 0)
            break;
        void *temp = heap->data[index];
        heap->data[index] = heap->data[parent];
        heap->data[parent] = temp;
        index = parent;
    }
}

static inline void CHeap_heapify_down(CHeap_t *heap, size_t index) {
    while (index * 2 + 1 < heap->size) {
        size_t left = index * 2 + 1;
        size_t right = index * 2 + 2;
        size_t smallest = index;

        if (heap->cmp(heap->data[left], heap->data[smallest]) < 0)
            smallest = left;
        if (right < heap->size &&
            heap->cmp(heap->data[right], heap->data[smallest]) < 0)
            smallest = right;
        if (smallest == index)
            break;

        void *temp = heap->data[index];
        heap->data[index] = heap->data[smallest];
        heap->data[smallest] = temp;
        index = smallest;
    }
}