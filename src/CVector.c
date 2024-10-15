#include <stdlib.h>
#include <string.h>

#include <logger/CLog.h>
#include <util/CVector.h>

struct _CVector {
    void **data;        ///< Array to store data.
    size_t size;        ///< Number of elements in the vector.
    size_t capacity;    ///< Capacity of the vector.
    Destructor destroy; ///< Function pointer to the destructor for cleaning up
                        ///< individual elements.
    char DYN_ALLOC; ///< whether it was dynamically allocated.
};

static size_t __ceil(double x) {
    size_t int_part = (size_t)x;
    if (x > int_part) {
        return int_part + 1;
    }
    CLog(DEBUG, "__ceil:%llu", int_part);
    return int_part;
}

static int alloc(CVector *vector) {
    if (vector == NULL)
        return CVECTOR_NULL_VECTOR;

    if (vector->data == NULL) {
        vector->data = malloc(CVECTOR_DEFAULT_ALLOC_SIZE * sizeof(void *));
        if (vector->data == NULL)
            return CVECTOR_ALLOC_FAILURE;
        vector->size = 0;
        vector->capacity = CVECTOR_DEFAULT_ALLOC_SIZE;
        return CVECTOR_SUCCESS;
    }

    if (vector->size == vector->capacity) {
        size_t new_size =
            __ceil((vector->capacity * CVECTOR_DEFAULT_GROWTH_RATE));

        CLog(DEBUG, "Making a realloc call in alloc.");
        void **data = realloc(vector->data, new_size * sizeof(void *));
        if (data == NULL)
            return CVECTOR_ALLOC_FAILURE;
        vector->data = data;
        vector->capacity = new_size;
    }

    if (vector->size > vector->capacity) {
        return CVECTOR_NULL_VECTOR;
    }

    return CVECTOR_SUCCESS;
}

size_t CVector_size(CVector *vector) {
    if (vector == NULL)
        return 0;
    return vector->size;
}

int CVector_init(CVector *vector, size_t reserve_capacity, Destructor destroy) {
    if (vector == NULL)
        return CVECTOR_NULL_VECTOR;
    size_t cap = reserve_capacity;
    vector->data = malloc(cap * sizeof(void *));
    if (vector->data == NULL)
        return CVECTOR_ALLOC_FAILURE;

    vector->size = 0;
    vector->capacity = cap;
    vector->destroy = destroy;

    return CVECTOR_SUCCESS;
}

CResult *CVector_new(size_t reserve_capacity, Destructor destroy) {
    CLog(DEBUG,
         "Making a malloc call in CVector_new with reserve capacity '%llu'.",
         reserve_capacity);
    CVector *vector = malloc(sizeof(CVector));
    if (vector == NULL)
        return CResult_ecreate(
            CError_create("Failed memory allocation for the vector.",
                          "CVector_new", CVECTOR_ALLOC_FAILURE));

    int code = CVector_init(vector, reserve_capacity, destroy);
    if (code)
        return CResult_ecreate(
            CError_create("Failed memory allocation for the vector's data.",
                          "CVector_new", code));
    return CResult_create(vector, NULL);
}

int CVector_add(CVector *vector, void *element) {
    int code;
    if (vector == NULL)
        return CVECTOR_NULL_VECTOR;

    if (vector->data == NULL)
        return CVECTOR_NULL_VECTOR;

    if (!(code = alloc(vector))) {
        vector->data[vector->size++] = element;
    }

    return code;
}

int CVector_del(CVector *vector, size_t index) {
    if (vector == NULL)
        return CVECTOR_NULL_VECTOR;
    if (index >= vector->size)
        return CVECTOR_INDEX_OUT_OF_BOUNDS;

    for (size_t i = index; i < vector->size - 1; ++i) {
        vector->data[i] = vector->data[i + 1];
    }

    vector->data[--vector->size] = NULL;
    return CVECTOR_SUCCESS;
}

CResult *CVector_get(const CVector *vector, size_t index) {
    if (vector == NULL)
        return CResult_ecreate(
            CError_create("Recieved a NULL pointer to the vector as source.",
                          "CVector_get", CVECTOR_NULL_VECTOR));
    if (index >= vector->size)
        return CResult_ecreate(
            CError_create("Index exceeds the size of the vector.",
                          "CVector_get", CVECTOR_INDEX_OUT_OF_BOUNDS));
    return CResult_create(vector->data[index], NULL);
}

size_t CVector_find(const CVector *vector, void *key, CompareTo cmp) {
    if (vector == NULL)
        return CVECTOR_NULL_VECTOR;
    for (size_t i = 0; i < vector->size; i++) {
        if (cmp(vector->data[i], key) == 0) {
            return i;
        }
    }
    return CVECTOR_INDEX_OUT_OF_BOUNDS;
}

static void insertion_sort(void **data, size_t left, size_t right,
                           CompareTo cmp) {
    for (size_t i = left + 1; i <= right; ++i) {
        void *key = data[i];
        size_t j = i;
        while (j > left && cmp(data[j - 1], key) > 0) {
            data[j] = data[j - 1];
            --j;
        }
        data[j] = key;
    }
}

static void merge(void **data, size_t l, size_t m, size_t r, CompareTo cmp) {
    size_t len1 = m - l + 1;
    size_t len2 = r - m;

    void **left = malloc(len1 * sizeof(void *));
    void **right = malloc(len2 * sizeof(void *));

    if (left == NULL || right == NULL) {
        free(left);
        free(right);
        return;
    }

    memcpy(left, &data[l], len1 * sizeof(void *));
    memcpy(right, &data[m + 1], len2 * sizeof(void *));

    size_t i = 0, j = 0, k = l;
    while (i < len1 && j < len2) {
        if (cmp(left[i], right[j]) <= 0) {
            data[k++] = left[i++];
        } else {
            data[k++] = right[j++];
        }
    }

    while (i < len1) {
        data[k++] = left[i++];
    }

    while (j < len2) {
        data[k++] = right[j++];
    }

    free(left);
    free(right);
}

// Thanks to Patrick Perry and other authors.
// I referenced this from
// https://github.com/patperry/timsort/blob/master/timsort.c
static void timsort(void **data, size_t size, CompareTo cmp) {
    const size_t min_run = 32;

    for (size_t i = 0; i < size; i += min_run) {
        size_t end = (i + min_run - 1 < size) ? i + min_run - 1 : size - 1;
        insertion_sort(data, i, end, cmp);
    }

    size_t run_size = min_run;
    while (run_size < size) {
        for (size_t start = 0; start < size; start += 2 * run_size) {
            size_t mid = start + run_size - 1;
            size_t end = (start + 2 * run_size - 1 < size)
                             ? start + 2 * run_size - 1
                             : size - 1;

            if (mid < end) {
                merge(data, start, mid, end, cmp);
            }
        }
        run_size *= 2;
    }
}

int CVector_sort(CVector *vector, CompareTo cmp) {
    if (vector == NULL)
        return CVECTOR_NULL_VECTOR;
    if (cmp == NULL)
        return CVECTOR_SORT_FAILURE;
    if (vector->size < 2)
        return CVECTOR_SUCCESS;
    timsort(vector->data, vector->size, cmp);
    return CVECTOR_SUCCESS;
}

int CVector_clear(CVector *vector) {
    if (vector == NULL)
        return CVECTOR_NULL_VECTOR;
    if (vector->destroy != NULL) {
        for (size_t i = 0; i < vector->size; ++i) {
            if (vector->data[i] != NULL) {
                vector->destroy(vector->data[i]);
                vector->data[i] = NULL;
            }
        }
    }

    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
    return CVECTOR_SUCCESS;
}

int CVector_free(CVector **vector) {
    if (vector == NULL || *vector == NULL)
        return CVECTOR_SUCCESS;
    int code = CVector_clear(*vector);
    if (code)
        return code;

    free(*vector);
    *vector = NULL;
    return CVECTOR_SUCCESS;
}

CResult *CVector_clone(const CVector *source, CloneFn cloner) {
    // NULL CVector indicates error somewhere down the line.
    if (source == NULL)
        return CResult_ecreate(
            CError_create("Recieved nullptr as source vector.", "CVector_copy",
                          CVECTOR_NULL_VECTOR));

    // NULL data is fine
    if (source->data == NULL || source->size == 0)
        return CVector_new(source->size, source->destroy);

    CLog(DEBUG, "Making a malloc call in CVector_copy for copy CVector.");
    CVector *copy = malloc(sizeof(CVector));

    if (copy == NULL) {
        return CResult_ecreate(
            CError_create("Unable to allocate memory for copying data.",
                          "CVector_copy", CVECTOR_ALLOC_FAILURE));
    }

    if (CVector_init(copy, source->capacity, source->destroy)) {
        free(copy);
        return CResult_ecreate(
            CError_create("Unable to initialize the new copy vector.",
                          "CVector_copy", CVECTOR_ALLOC_FAILURE));
    }
    if (cloner == NULL) {
        free(copy);
        return CResult_ecreate(
            CError_create("Unable to clone the data to new copy vector..",
                          "CVector_copy", CVECTOR_ALLOC_FAILURE));
    }

    for (size_t i = 0; i < source->size; i++) {
        void *element = cloner(source->data[i]);
        if (CVector_add(copy, element) != CVECTOR_SUCCESS) {
            CVector_free(&copy);
            return CResult_ecreate(
                CError_create("Unable to copy elements from source to copy.",
                              "CVector_copy", CVECTOR_ALLOC_FAILURE));
        }
    }

    return CResult_create(copy, NULL);
}

int CVector_reserve(CVector *vector, size_t new_capacity) {
    if (vector == NULL) {
        return CVECTOR_NULL_VECTOR;
    }

    if (new_capacity <= vector->capacity) {
        return CVECTOR_SUCCESS;
    }

    CLog(DEBUG, "Making a realloc call in CVector_reserve.");
    void **new_data = realloc(vector->data, new_capacity * sizeof(void *));
    if (new_data == NULL) {
        return CVECTOR_ALLOC_FAILURE;
    }

    if (vector->capacity > 0) {
        memset(new_data + vector->capacity, 0,
               (new_capacity - vector->capacity) * sizeof(void *));
    }

    vector->data = new_data;
    vector->capacity = new_capacity;

    return CVECTOR_SUCCESS;
}
