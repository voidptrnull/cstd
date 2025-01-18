#include <assert.h>
#include <cstd/CHeap.h>
#include <cstd/CLog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_integers(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int test_heap_creation() {
    CLog(INFO, "test_heap_creation()");
    CResult_t *res = CHeap_new(10, compare_integers);
    assert(!CResult_is_error(res));
    CHeap_t *heap = CResult_get(res);

    assert(CHeap_size(heap) == 0);
    CHeap_free(&heap);
    CResult_free(&res);

    return 0;
}

int test_heap_insertion() {
    CLog(INFO, "test_heap_insertion()");
    CResult_t *res = CHeap_new(10, compare_integers);
    assert(!CResult_is_error(res));
    CHeap_t *heap = CResult_get(res);

    int nums[] = {10, 20, 5, 30, 15};
    for (int i = 0; i < 5; i++) {
        assert(CHeap_insert(heap, &nums[i]) == CHEAP_SUCCESS);
    }

    assert(CHeap_size(heap) == 5);
    CHeap_free(&heap);
    CResult_free(&res);

    return 0;
}

int test_heap_extraction() {
    CLog(INFO, "test_heap_extraction()");
    CResult_t *res = CHeap_new(10, compare_integers);
    assert(!CResult_is_error(res));
    CHeap_t *heap = CResult_get(res);

    int nums[] = {10, 20, 5, 30, 15};
    for (int i = 0; i < 5; i++) {
        assert(CHeap_insert(heap, &nums[i]) == CHEAP_SUCCESS);
    }

    int *min_elem = CHeap_fextract(heap);
    assert(CHeap_insert(heap, &nums[4]) == CHEAP_SUCCESS);
    CResult_t *r = CHeap_extract(heap);
    assert(!CResult_is_error(r));
    int *nmin_elem = CResult_get(r);
    CResult_free(&r);
    assert(*min_elem == 5);
    assert(*nmin_elem == 10);
    assert(CHeap_size(heap) == 4);
    CHeap_free(&heap);
    CResult_free(&res);

    return 0;
}

int test_heap_resize() {
    CLog(INFO, "test_heap_resize()");
    CResult_t *res = CHeap_new(5, compare_integers);
    assert(!CResult_is_error(res));
    CHeap_t *heap = CResult_get(res);

    int nums[] = {10, 20, 5, 30, 15};
    for (int i = 0; i < 5; i++) {
        assert(CHeap_insert(heap, &nums[i]) == CHEAP_SUCCESS);
    }

    assert(CHeap_resize(heap, 10) == CHEAP_SUCCESS);
    assert(CHeap_size(heap) == 5);

    CHeap_free(&heap);
    CResult_free(&res);

    return 0;
}

int main() {
    enable_location();
    shortened_location();
    assert(!test_heap_creation());
    assert(!test_heap_insertion());
    assert(!test_heap_extraction());
    assert(!test_heap_resize());

    return 0;
}
