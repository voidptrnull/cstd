#include <CVector.h>
#include <stdlib.h>
#include <string.h>

inline size_t min_capacity(size_t cap) {
	size_t newSize = cap;
	newSize |= newSize >> 1;
	newSize |= newSize >> 2;
	newSize |= newSize >> 4;
	newSize |= newSize >> 8;
	newSize |= newSize >> 16;
	if (sizeof(newSize) > 4)
		newSize |= newSize >> 32;
	newSize++;
	newSize = (newSize <= cap) ? newSize : cap;
	if (newSize == 0) {
		newSize = cap;
	}
	return newSize;
}

inline int alloc(CVector* vector) {
	if (vector == NULL) return CVECTOR_NULL_VECTOR;

	if (vector->data == NULL) {
		vector->data = malloc(CVECTOR_DEFAULT_ALLOC_SIZE * sizeof(void*));
    	if (vector->data == NULL) return CVECTOR_ALLOC_FAILURE;
		vector->size = 0;
    	vector->capacity = CVECTOR_DEFAULT_ALLOC_SIZE;
     	return CVECTOR_SUCCESS;
	}

	if (vector->size == vector->capacity) {
		size_t new_size = (size_t)(vector->capacity * CVECTOR_DEFAULT_GROWTH_RATE);
		void** data = realloc(vector->data, new_size * sizeof(void*));
		if (data == NULL)
			return CVECTOR_ALLOC_FAILURE;
		vector->data = data;
		vector->capacity = new_size;
	}

	return CVECTOR_SUCCESS;
}

int CVector_init(CVector* vector, size_t reserve_capacity) {
	if (vector == NULL) return CVECTOR_NULL_VECTOR;
	size_t cap = min_capacity(reserve_capacity);
    vector->data = malloc(cap * sizeof(void*));
    if (vector->data == NULL )
    	return CVECTOR_ALLOC_FAILURE;

    vector->size = 0;
    vector->capacity = cap;

    return CVECTOR_SUCCESS;
}

CResult* CVector_new(size_t reserve_capacity) {
	CVector* vector = malloc(sizeof(CVector));
    if (vector == NULL) 
        return CResult_ecreate(CError_create("Failed memory allocation for the vector.", "CVector_new", CVECTOR_ALLOC_FAILURE));

    int code = CVector_init(vector, reserve_capacity);
    if (code)
        return CResult_ecreate(CError_create("Failed memory allocation for the vector's data.", "CVector_new", code));
    return CResult_create(vector);
}

int CVector_add(CVector* vector, void* element) {
	int code;
	if (!(code = alloc(vector))) {
		vector->data[vector->size++] = element;
	}

	return code;
}

int CVector_del(CVector* vector, size_t index) {
	if (vector == NULL) return CVECTOR_NULL_VECTOR;
	if (index >= vector->size) return CVECTOR_INDEX_OUT_OF_BOUNDS;

	for (size_t i = index; i < vector->size - 1; ++i) {
        vector->data[i] = vector->data[i + 1];
    }

    vector->data[--vector->size] = NULL;
    return CVECTOR_SUCCESS;
}

CResult* CVector_get(const CVector* vector, size_t index) {
	if (vector == NULL) return CResult_ecreate(CError_create("Recieved a NULL ptr to a vector as source.", "CVector_get", CVECTOR_NULL_VECTOR));
	if (index >= vector->size) return CResult_ecreate(CError_create("Index exceeds the size of the vector.", "CVector_get", CVECTOR_INDEX_OUT_OF_BOUNDS));
    if (index < 0) return CResult_ecreate(CError_create("Index cannot be smaller than 0.", "CVector_get", CVECTOR_INDEX_OUT_OF_BOUNDS));
	return CResult_create(vector->data[index]);
}

size_t CVector_find(const CVector* vector, void* key, CompareTo cmp) {
	if (vector == NULL) return CVECTOR_NULL_VECTOR;
	for (size_t i = 0; i < vector->size; i++) {
		if (cmp(vector->data[i], key) == 0) {
			return i;
		}
	}
	return CVECTOR_INDEX_OUT_OF_BOUNDS;
}

static void insertion_sort(void** data, size_t left, size_t right, CompareTo cmp) {
    for (size_t i = left + 1; i <= right; ++i) {
        void* key = data[i];
        size_t j = i;
        while (j > left && cmp(data[j - 1], key) > 0) {
            data[j] = data[j - 1];
            --j;
        }
        data[j] = key;
    }
}

static void merge(void** data, size_t l, size_t m, size_t r, CompareTo cmp) {
    size_t len1 = m - l + 1;
    size_t len2 = r - m;

    void** left = malloc(len1 * sizeof(void*));
    void** right = malloc(len2 * sizeof(void*));

    if (left == NULL || right == NULL) {
        free(left);
        free(right);
        return;
    }

    memcpy(left, &data[l], len1 * sizeof(void*));
    memcpy(right, &data[m + 1], len2 * sizeof(void*));

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
// I referenced this from https://github.com/patperry/timsort/blob/master/timsort.c
static void timsort(void** data, size_t size, CompareTo cmp) {
    const size_t min_run = 32;

    for (size_t i = 0; i < size; i += min_run) {
        size_t end = (i + min_run - 1 < size) ? i + min_run - 1 : size - 1;
        insertion_sort(data, i, end, cmp);
    }

    size_t run_size = min_run;
    while (run_size < size) {
        for (size_t start = 0; start < size; start += 2 * run_size) {
            size_t mid = start + run_size - 1;
            size_t end = (start + 2 * run_size - 1 < size) ? start + 2 * run_size - 1 : size - 1;

            if (mid < end) {
                merge(data, start, mid, end, cmp);
            }
        }
        run_size *= 2;
    }
}

int CVector_sort(CVector* vector, CompareTo cmp) {
	if (vector == NULL) return CVECTOR_NULL_VECTOR;
	if (cmp == NULL)
		return CVECTOR_SORT_FAILURE;
	if (vector->size < 2) return CVECTOR_SUCCESS;
	timsort(vector->data, vector->size, cmp);
	return CVECTOR_SUCCESS;
}

int CVector_clear(CVector* vector) {
	if (vector == NULL) return CVECTOR_NULL_VECTOR;
	if (vector->data != NULL) {
		free(vector->data);
		vector->data = NULL;
	}

	vector->size = 0;
	vector->capacity = 0;
	return CVECTOR_SUCCESS;
}

int CVector_free(CVector* vector) {
    if (vector == NULL) return CVECTOR_SUCCESS;
    int code = CVector_clear(vector);
    if (code) return code;
    
    free(vector);
    return CVECTOR_SUCCESS;
}

CResult* CVector_copy(const CVector* source) {
    // NULL CVector indicates error somewhere down the line.
    if (source == NULL) 
        return CResult_ecreate(CError_create("Recieved nullptr as source vector.", "CVector_copy", CVECTOR_NULL_VECTOR));
    
    // NULL data is fine
    if (source->data == NULL || source->size == 0) 
        return CVector_new(source->size);
    
    CVector* copy = malloc(sizeof(CVector));
    
    if (copy == NULL) {
        return CResult_ecreate(CError_create("Unable to allocate memory for copying data.", "CVector_copy", CVECTOR_ALLOC_FAILURE));
    }
    
    if (CVector_init(copy, source->capacity)) {
        free(copy);
        return CResult_ecreate(CError_create("Unable to initialize the new copy vector.", "CVector_copy", CVECTOR_ALLOC_FAILURE));
    }

    for (size_t i = 0;i <= source->size;i++) {
        void* element = CVector_get(source, i);
        if (CVector_add(copy, element) != CVECTOR_SUCCESS) {
            CVector_free(copy);
            return CResult_ecreate(CError_create("Unable to copy elements from source to copy.", "CVector_copy", CVECTOR_ALLOC_FAILURE));
        }
    }

    return CResult_create(copy);
}

int CVector_reserve(CVector* vector, size_t new_capacity) {
    if (vector == NULL) {
        return CVECTOR_NULL_VECTOR;
    }

    if (new_capacity <= vector->capacity) {
        return CVECTOR_SUCCESS;
    }

    void** new_data = (void**)realloc(vector->data, new_capacity * sizeof(void*));
    if (new_data == NULL) {
        return CVECTOR_ALLOC_FAILURE;
    }

    memset(new_data + vector->capacity, 0, (new_capacity - vector->capacity) * sizeof(void*));

    vector->data = new_data;
    vector->capacity = new_capacity;

    return CVECTOR_SUCCESS;
}
