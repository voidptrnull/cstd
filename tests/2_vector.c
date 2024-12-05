#include <assert.h>
#include <cstd/CLog.h>
#include <cstd/CVector.h>
#include <stdlib.h>
#include <string.h>

int test_strings() {
    CLog(INFO, "test_strings()");
    CResult_t *res = CVector_new(10, free);
    assert(!CResult_is_error(res));

    CVector_t *vec = CResult_get(res);
    const char *test_strings[] = {"Hello", "World", "CVector", "Test"};

    for (int i = 0; i < 4; i++) {
        CVector_add(vec, strdup(test_strings[i]));
    }

    for (int i = 0; i < 4; i++) {
        CResult_t *res1 = CVector_get(vec, i);
        assert(!CResult_is_error(res1));
        const char *s = CResult_get(res1);
        assert(strcmp(s, test_strings[i]) == 0);
        CResult_free(&res1);
    }

    CVector_free(&vec);
    CResult_free(&res);
    return 0;
}

int test_floats() {
    CLog(INFO, "test_floats()");
    CResult_t *res = CVector_new(10, free);
    assert(!CResult_is_error(res));

    CVector_t *vec = CResult_get(res);
    float test_floats[] = {1.1f, 2.2f, 3.3f};

    for (int i = 0; i < 3; i++) {
        float *f = malloc(sizeof(float));
        assert(f != NULL);
        *f = test_floats[i];
        CVector_add(vec, f);
    }

    for (int i = 0; i < 3; i++) {
        CResult_t *res1 = CVector_get(vec, i);
        assert(!CResult_is_error(res1));
        float *f = CResult_get(res1);
        assert(*f == test_floats[i]);
        CResult_free(&res1);
    }

    CVector_free(&vec);
    CResult_free(&res);
    return 0;
}

/* Just a demo-struct for testing custom data. Simple but efficient enough. */
typedef struct _Student {
    int64_t id;
    char *name;
} Student;

int test_custom_structs() {
    CLog(INFO, "test_custom_structs()");
    CResult_t *res = CVector_new(10, free);
    assert(!CResult_is_error(res));

    CVector_t *vec = CResult_get(res);

    for (int i = 0; i < 10; i++) {
        Student *s = malloc(sizeof(Student));
        assert(s != NULL);
        s->id = i;
        s->name = "NAME";
        CVector_add(vec, s);
    }

    for (int i = 0; i < 10; i++) {
        CResult_t *res1 = CVector_get(vec, i);
        assert(!CResult_is_error(res1));
        Student *s = CResult_get(res1);
        assert(s->id == i);
        assert(strcmp(s->name, "NAME") == 0);
        CResult_free(&res1);
    }

    CVector_free(&vec);
    CResult_free(&res);
    return 0;
}

int test_empty_vector() {
    CLog(INFO, "test_empty_vector()");
    CResult_t *res = CVector_new(0, free);
    assert(!CResult_is_error(res));

    CVector_t *vec = CResult_get(res);
    assert(CVector_size(vec) == 0);

    CResult_t *res1 = CVector_get(vec, 0);
    assert(CResult_is_error(res1));

    CVector_free(&vec);
    CResult_free(&res);
    CResult_free(&res1);
    return 0;
}

int test_capacity_growth() {
    CLog(INFO, "test_capacity_growth()");
    CResult_t *res = CVector_new(2, free);
    assert(!CResult_is_error(res));

    CVector_t *vec = CResult_get(res);
    for (int i = 0; i < 5; i++) {
        int *num = malloc(sizeof(int));
        assert(num != NULL);
        *num = i;
        if (CVector_add(vec, num)) {
            return 1;
        }
    }

    assert(CVector_size(vec) == 5);

    for (int i = 0; i < 5; i++) {
        CResult_t *res1 = CVector_get(vec, i);
        assert(!CResult_is_error(res1));
        int *num = CResult_get(res1);
        assert(*num == i);
        CResult_free(&res1);
    }

    CVector_free(&vec);
    CResult_free(&res);
    return 0;
}

int test_clear() {
    CLog(INFO, "test_clear()");
    CResult_t *res = CVector_new(5, free);
    assert(!CResult_is_error(res));

    CVector_t *vec = CResult_get(res);
    for (int i = 0; i < 5; i++) {
        int *num = malloc(sizeof(int));
        assert(num != NULL);
        *num = i;
        CVector_add(vec, num);
    }

    assert(CVector_size(vec) == 5);
    assert(CVector_clear(vec) == CVECTOR_SUCCESS);
    assert(CVector_size(vec) == 0);

    CVector_free(&vec);
    CResult_free(&res);
    return 0;
}

int test_free() {
    CLog(INFO, "test_free()");
    CResult_t *res = CVector_new(5, free);
    assert(!CResult_is_error(res));

    CVector_t *vec = CResult_get(res);
    for (int i = 0; i < 5; i++) {
        int *num = malloc(sizeof(int));
        assert(num != NULL);
        *num = i;
        CVector_add(vec, num);
    }

    assert(CVector_size(vec) == 5);
    assert(CVector_free(&vec) == CVECTOR_SUCCESS);
    assert(vec == NULL); // Ensure the pointer is set to NULL
    CResult_free(&res);
    return 0;
}

#include <stdint.h>

void *cclone_integer(const void *data) {
    if (data == NULL)
        return NULL;
    int *clone = malloc(sizeof(int));
    if (clone) {
        *clone = *(intmax_t *)data;
    }
    return clone;
}

int test_copy() {
    CLog(INFO, "test_copy()");
    CResult_t *res = CVector_new(5, free);
    assert(!CResult_is_error(res));

    CVector_t *vec = CResult_get(res);
    for (int i = 0; i < 5; i++) {
        int *num = malloc(sizeof(int));
        assert(num != NULL);
        *num = i;
        CVector_add(vec, num);
    }

    CResult_t *copy_result = CVector_clone(vec, cclone_integer);
    assert(!CResult_is_error(copy_result));
    CVector_t *vec_copy = CResult_get(copy_result);
    assert(CVector_size(vec_copy) == CVector_size(vec));

    for (int64_t i = 0; i < CVector_size(vec); i++) {
        CLog(DEBUG, "Accessing element at '%d'.", i);
        CResult_t *res1 = CVector_get(vec, i);
        CResult_t *res2 = CVector_get(vec_copy, i);
        assert(!CResult_is_error(res1));
        assert(!CResult_is_error(res2));
        int *num1 = CResult_get(res1);
        int *num2 = CResult_get(res2);
        CLog(DEBUG, "Num 1 is: '%d'.", *num1);
        CLog(DEBUG, "Num 2 is: '%d'.", *num2);

        assert(*num1 == *num2);
        CResult_free(&res1);
        CResult_free(&res2);
    }

    CVector_free(&vec_copy);
    CVector_free(&vec);
    CResult_free(&copy_result);
    CResult_free(&res);
    return 0;
}

#include <time.h>

int cmp(const void *a, const void *b) {
    uint64_t va = *(uint64_t *)a;
    uint64_t vb = *(uint64_t *)b;
    if (va < vb)
        return -1;
    if (va > vb)
        return 1;
    return 0;
}

int test_sort() {
    CLog(INFO, "test_sort()");
    CResult_t *res = CVector_new(300, free);
    assert(!CResult_is_error(res));
    CVector_t *vec = CResult_get(res);
    for (int64_t i = 1; i < 2000; i++) {
        uint64_t *e = malloc(sizeof(uint64_t));
        assert(e != NULL);
        *e = UINT64_MAX / i;
        assert(CVector_add(vec, e) == CVECTOR_SUCCESS);
    }
    assert(CVector_sort(vec, cmp) == CVECTOR_SUCCESS);
    for (int64_t i = 1; i < CVector_size(vec); i++) {
        CResult_t *res1 = CVector_get(vec, i);
        assert(!CResult_is_error(res1));
        CResult_t *res2 = CVector_get(vec, i - 1);
        assert(!CResult_is_error(res2));
        assert(*(uint64_t *)CResult_get(res1) >=
               *(uint64_t *)CResult_get(res2));
        CResult_free(&res1);
        CResult_free(&res2);
    }
    CVector_free(&vec);
    CResult_free(&res);
    return 0;
}

int test_reserve() {
    CLog(INFO, "test_reserve()");
    CResult_t *res = CVector_new(20, NULL);
    assert(!CResult_is_error(res));
    CVector_t *vec = CResult_get(res);
    assert(!CVector_reserve(vec, 100));
    for (int i = 0; i < 100; i++) {
        CVector_add(vec, &i);
    }
    assert(CVector_size(vec) == 100);
    CVector_free(&vec);
    CResult_free(&res);
    return 0;
}

int main() {
    // enable_debugging();
    enable_location();
    shortened_location();
    assert(!test_sort());
    assert(!test_strings());
    assert(!test_floats());
    assert(!test_custom_structs());
    assert(!test_empty_vector());
    assert(!test_capacity_growth());
    assert(!test_clear());
    assert(!test_free());
    assert(!test_copy());
    assert(!test_reserve());

    return 0;
}