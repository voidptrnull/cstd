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

#include <assert.h>
#include <cstd/CLinkedList.h>
#include <cstd/CLog.h>
#include <stdlib.h>
#include <string.h>

int scmp(const void *s1, const void *s2) {
    return strcmp((const char *)s1, (const char *)s2);
}

int test_strings() {
    CLog(INFO, "test_strings()");
    CResult_t *res =
        CLinkedList_new(CLINKEDLIST_TYPE_SINGLE, free); // Singly linked list
    assert(!CResult_is_error(res));
    CLinkedList_t *list = CResult_get(res);
    char *test_strings[] = {"Hello", "World", "CLinkedList", "Test"};

    for (int i = 0; i < 4; i++) {
        assert(!CLinkedList_add(list, strdup(test_strings[i])));
    }

    for (int i = 0; i < 4; i++) {
        CResult_t *res1 = CLinkedList_get(list, i);
        assert(!CResult_is_error(res1));
        char *s = CResult_get(res1);
        assert(strcmp(s, test_strings[i]) == 0);
        CResult_free(&res1);
    }

    int to_find = rand() % 4;
    assert(CLinkedList_find(list, test_strings[to_find], scmp) == to_find);

    CLinkedList_free(&list);
    CResult_free(&res);
    return 0;
}

int test_integers() {
    CLog(INFO, "test_integers()");
    CResult_t *res =
        CLinkedList_new(CLINKEDLIST_TYPE_DOUBLE, free); // Doubly linked list
    assert(!CResult_is_error(res));

    CLinkedList_t *list = CResult_get(res);
    int test_integers[] = {1, 2, 3, 4};

    for (int i = 0; i < 4; i++) {
        int *num = malloc(sizeof(int));
        assert(num != NULL);
        *num = test_integers[i];
        CLinkedList_add(list, num);
    }

    for (int i = 0; i < 4; i++) {
        CResult_t *res1 = CLinkedList_get(list, i);
        assert(!CResult_is_error(res1));
        int *num = CResult_get(res1);
        assert(*num == test_integers[i]);
        CResult_free(&res1);
    }

    CLinkedList_free(&list);
    CResult_free(&res);
    return 0;
}

typedef struct _Student {
    int id;
    char *name;
} Student;

int test_custom_structs() {
    CLog(INFO, "test_custom_structs()");
    CResult_t *res = CLinkedList_new(CLINKEDLIST_TYPE_SINGLE, free);
    assert(!CResult_is_error(res));

    CLinkedList_t *list = CResult_get(res);

    for (int i = 0; i < 10; i++) {
        Student *student = malloc(sizeof(Student));
        assert(student != NULL);
        student->id = i;
        student->name = strdup("NAME");
        CLinkedList_add(list, student);
    }

    for (int i = 0; i < 10; i++) {
        CResult_t *res1 = CLinkedList_get(list, i);
        assert(!CResult_is_error(res1));
        Student *student = CResult_get(res1);
        assert(student->id == i);
        assert(strcmp(student->name, "NAME") == 0);
        CResult_free(&res1);
    }

    CLinkedList_free(&list);
    CResult_free(&res);
    return 0;
}

int test_clear() {
    CLog(INFO, "test_clear()");
    CResult_t *res =
        CLinkedList_new(CLINKEDLIST_TYPE_DOUBLE, free); // Doubly linked list
    assert(!CResult_is_error(res));

    CLinkedList_t *list = CResult_get(res);
    for (int i = 0; i < 5; i++) {
        int *num = malloc(sizeof(int));
        assert(num != NULL);
        *num = i;
        CLinkedList_add(list, num);
    }

    assert(CLinkedList_size(list) == 5);
    assert(CLinkedList_clear(list) == CLINKEDLIST_SUCCESS);
    assert(CLinkedList_size(list) == 0);

    CLinkedList_free(&list);
    CResult_free(&res);
    return 0;
}

void *cclone_integer(const void *data) {
    if (!data)
        return NULL;
    int *clone = malloc(sizeof(int));
    assert(clone != NULL);
    *clone = *(const int *)data;
    return clone;
}

int test_clone() {
    CLog(INFO, "test_clone()");
    CResult_t *res =
        CLinkedList_new(CLINKEDLIST_TYPE_SINGLE, free); // Singly linked list
    assert(!CResult_is_error(res));

    CLinkedList_t *list = CResult_get(res);

    for (int i = 0; i < 5; i++) {
        int *num = malloc(sizeof(int));
        assert(num != NULL);
        *num = i;
        CLinkedList_add(list, num);
    }

    CResult_t *clone_res = CLinkedList_clone(list, cclone_integer);
    assert(!CResult_is_error(clone_res));

    CLinkedList_t *cloned_list = CResult_get(clone_res);
    assert(CLinkedList_size(cloned_list) == CLinkedList_size(list));

    for (int i = 0; i < 5; i++) {
        CResult_t *orig_res = CLinkedList_get(list, i);
        CResult_t *clone_res1 = CLinkedList_get(cloned_list, i);
        assert(!CResult_is_error(orig_res));
        assert(!CResult_is_error(clone_res1));
        int *orig = CResult_get(orig_res);
        int *clone = CResult_get(clone_res1);
        assert(*orig == *clone);
        CResult_free(&orig_res);
        CResult_free(&clone_res1);
    }

    CLinkedList_free(&cloned_list);
    CResult_free(&clone_res);
    CLinkedList_free(&list);
    CResult_free(&res);
    return 0;
}

int main() {
    enable_location();
    shortened_location();
    assert(!test_strings());
    assert(!test_integers());
    assert(!test_custom_structs());
    assert(!test_clear());
    assert(!test_clone());
    return 0;
}
