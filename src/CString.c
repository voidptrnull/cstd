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

#include <cstd/CString.h>
#include <stdlib.h>
#include <string.h>

struct _CString {
    CVector_t *characters;
};

CResult_t *CString_new() {
    CString_t *string = malloc(sizeof(CString_t));

    int code = CString_init(string, CSTRING_DEFAULT_ALLOC_SIZE);
    if (code) {
        return CResult_ecreate(CError_create(
            "Initialization of CString returned non-zero exit code.",
            "CString_new", code));
    }

    return CResult_create(string, NULL);
}

int CString_init(CString_t *string, size_t size) {
    if (string == NULL)
        return CSTRING_NULL_STRING;

    CResult_t *res = CVector_new(size, NULL);
    if (CResult_is_error(res)) {
        CResult_free(&res);
        string->characters = NULL;
        return CSTRING_ALLOC_FAILURE;
    }
    string->characters = CResult_get(res);
    CResult_free(&res);
    return CSTRING_SUCCESS;
}

int CString_set(CString_t *string, char *str) {
    if (string == NULL || str == NULL)
        return CSTRING_NULL_STRING;

    int code = CString_clear(string);
    if (code != 0)
        return code;

    size_t len = strlen(str);
    code = CString_init(string, len);
    if (code)
        return code;
    for (size_t i = 0; i < len; i++) {
        code = CVector_add(string->characters, &str[i]);
        if (code != CVECTOR_SUCCESS) {
            return CSTRING_OP_FAILURE;
        }
    }

    return CSTRING_SUCCESS;
}

char CString_at(const CString_t *string, size_t index) {
    if (string == NULL || string->characters == NULL)
        return '\0';

    if (index >= CVector_size(string->characters))
        return '\0';

    CResult_t *res = CVector_get(string->characters, index);
    if (CResult_is_error(res)) {
        CResult_free(&res);
        return '\0';
    }
    char c = (char)CResult_get(res);
    CResult_free(&res);
    return c;
}

int CString_free(CString_t **string) {
    if (string == NULL || *string == NULL)
        return CSTRING_SUCCESS;
    int code = CString_clear(*string);
    if (code)
        return code;
    free(*string);
    *string = NULL;
    return CSTRING_SUCCESS;
}

size_t CString_length(const CString_t *string) {
    return CVector_size(string->characters);
}

int CString_append_c(CString_t *string, const char *str) {
    if (string == NULL || str == NULL)
        return CSTRING_NULL_STRING;

    size_t append_len = strlen(str);
    size_t current_len = CVector_size(string->characters);

    if (CVector_reserve(string->characters, current_len + append_len) !=
        CVECTOR_SUCCESS)
        return CSTRING_ALLOC_FAILURE;

    for (size_t i = 0; i < append_len; i++)
        if (CVector_add(string->characters, (void *)(uintptr_t)(str[i])) !=
            CVECTOR_SUCCESS)
            return CSTRING_ALLOC_FAILURE;

    return CSTRING_SUCCESS;
}

int CString_append(CString_t *string, CString_t *str) {
    if (string == NULL || str == NULL)
        return CSTRING_NULL_STRING;

    size_t append_len = CVector_size(str->characters);
    size_t current_len = CVector_size(string->characters);

    if (CVector_reserve(string->characters, current_len + append_len) !=
        CVECTOR_SUCCESS)
        return CSTRING_ALLOC_FAILURE;

    for (size_t i = 0; i < append_len; i++) {
        CResult_t *res = CVector_get(str->characters, i);
        if (CResult_is_error(res) ||
            CVector_add(string->characters, CResult_get(res)) !=
                CVECTOR_SUCCESS)
            return CSTRING_ALLOC_FAILURE;
        // don't delete data, just free the pointer to result.
        CResult_free(&res);
    }
    return CSTRING_SUCCESS;
}

void *clone_char(const void *data) {
    char *ch = malloc(sizeof(char));
    if (ch == NULL)
        return NULL;

    if (data == NULL || (*(uint8_t *)data & ~0x7F) != 0) {
        *ch = '\0';
        return ch;
    }

    *ch = *(const uint8_t *)data;
    return ch;
}

CResult_t *CString_clone(const CString_t *source) {
    if (source == NULL)
        return NULL;

    CString_t *copy = malloc(sizeof(CString_t));
    if (copy == NULL)
        return NULL;

    if (source->characters == NULL)
        return CResult_create(copy, free);

    CResult_t *res = CVector_clone(source->characters, clone_char);
    if (CResult_is_error(res))
        return res;
    copy->characters = CResult_get(res);
    CResult_free(&res);
    return CResult_create(copy, free);
}

int CString_clear(CString_t *string) {
    if (string == NULL)
        return CSTRING_NULL_STRING;

    if (string->characters != NULL) {
        int code = CVector_clear(string->characters);
        if (code != CVECTOR_SUCCESS)
            return CSTRING_OP_FAILURE;
    }

    if (string->characters) {
        return CVector_free(&string->characters);
    }

    return CSTRING_SUCCESS;
}

CResult_t *CString_c_str(CString_t *string) {
    if (string == NULL)
        return CResult_ecreate(
            CError_create("Got null CString as source, unable to convert it to "
                          "C-style strings.",
                          "CString_c_str", CSTRING_NULL_STRING));

    if (CVector_size(string->characters) <= 0 || string->characters == NULL) {
        char *empty_str = malloc(1);
        if (!empty_str) {
            return CResult_ecreate(
                CError_create("Memory allocation failed for empty string.",
                              "CString_c_str", CSTRING_ALLOC_FAILURE));
        }
        empty_str[0] = '\0';
        return CResult_create(empty_str, free);
    }
    char *str = malloc(sizeof(char) * (CVector_size(string->characters) + 1));
    if (str == NULL)
        return CResult_ecreate(
            CError_create("Unable to allocate memory for C string.",
                          "CString_c_str", CSTRING_ALLOC_FAILURE));
    for (size_t i = 0; i < CVector_size(string->characters); i++) {
        CResult_t *res = CVector_get(string->characters, i);
        if (CResult_is_error(res)) {
            free(str);
            CResult_free(&res);
            return CResult_ecreate(CError_create("Unable to get the character.",
                                                 "CString_c_str",
                                                 CSTRING_OP_FAILURE));
        }
        str[i] = (char)CResult_get(res);
        CResult_free(&res);
    }
    str[CVector_size(string->characters)] = '\0'; // Null terminator

    return CResult_create((void *)str, free);
}

int CString_equals(CString_t *str1, CString_t *str2) {
    if (str1 == str2)
        return 1;
    if (str1 == NULL || str2 == NULL)
        return 0;

    if (CVector_size(str1->characters) != CVector_size(str2->characters))
        return 0;

    for (size_t i = 0; i < CVector_size(str1->characters); i++) {
        CResult_t *res1 = CVector_get(str1->characters, i);
        CResult_t *res2 = CVector_get(str2->characters, i);

        char x, y;
        if (CResult_is_error(res1))
            return 0;
        x = *(char *)CResult_get(res1);

        if (CResult_is_error(res2))
            return 0;
        y = *(char *)CResult_get(res2);

        if (x != y)
            return 0;
        CResult_free(&res1);
        CResult_free(&res2);
    }

    return 1;
}

int64_t CString_compare(CString_t *str1, CString_t *str2) {
    if (str1 == str2)
        return 0;
    if (str1 == NULL || str2 == NULL)
        return INT64_MIN;

    if (CString_length(str1) - CString_length(str2))
        return CString_length(str1) - CString_length(str2);

    for (size_t i = 0; i < CString_length(str1); i++) {
        if (CString_at(str1, i) != CString_at(str2, i)) {
            return CString_at(str1, i) - CString_at(str2, i);
        }
    }

    return 0;
}

CResult_t *CString_substring(const CString_t *string, size_t start,
                             size_t end) {
    if (!string || !string->characters) {
        return CResult_ecreate(
            CError_create("Failed to add character to substring.",
                          "CString_substring", CSTRING_NULL_STRING));
    }

    size_t length = CString_length(string);

    if (start >= length || end >= length || start > end) {
        return CResult_ecreate(
            CError_create("Index of substring is out of bounds.",
                          "CString_substring", CSTRING_INDEX_OUT_OF_BOUNDS));
    }

    size_t substring_length = end - start + 1;
    CResult_t *res = CVector_new(substring_length, NULL);
    if (CResult_is_error(res)) {
        CResult_free(&res);
        return CResult_ecreate(
            CError_create("An error was encountered during creating a new "
                          "vector for storing the substring.",
                          "CString_substring", CSTRING_ALLOC_FAILURE));
    }
    CVector_t *substring_vector = CResult_get(res);
    if (!substring_vector) {
        return CResult_ecreate(
            CError_create("Failed to allocate memory for substring vector.",
                          "CString_substring", CSTRING_ALLOC_FAILURE));
    }

    for (size_t i = start; i <= end; ++i) {
        char ch = CString_at(string, i);
        if (CVector_add(substring_vector, (void *)(uintptr_t)ch) !=
            CVECTOR_SUCCESS) {
            CVector_free(&substring_vector);
            return CResult_ecreate(
                CError_create("Failed to add character to substring.",
                              "CString_substring", CSTRING_ALLOC_FAILURE));
        }
    }

    CString_t *substring = malloc(sizeof(CString_t));
    if (!substring) {
        CVector_free(&substring_vector);
        return CResult_ecreate(
            CError_create("Failed to allocate memory for substring.",
                          "CString_substring", CSTRING_ALLOC_FAILURE));
    }

    if (CString_init(substring, substring_length)) {
        free(substring);
        CVector_free(&substring_vector);
        return CResult_ecreate(
            CError_create("Failed to initialize substring's array.",
                          "CString_substring", CSTRING_ALLOC_FAILURE));
    }

    substring->characters = substring_vector;
    return CResult_create(substring, free);
}

#if __STDC_VERSION__ >= 201112L // C11 support

CResult_t *CString_c_wchar_t(CString_t *string) {
    if (!string || CString_length(string) == 0) {
        return CResult_ecreate(CError_create("Recieved a null string.",
                                             "CString_c_wchar_t",
                                             CSTRING_NULL_STRING));
    }

    size_t length = CString_length(string);
    wchar_t *wide_str = (wchar_t *)malloc((length + 1) * sizeof(wchar_t));
    if (!wide_str) {
        return CResult_ecreate(
            CError_create("Unable to allocate memory for string.",
                          "CString_c_utf8_str", CSTRING_ALLOC_FAILURE));
    }

    for (size_t i = 0; i < length; ++i) {
        wide_str[i] = (wchar_t)CString_at(string, i);
    }

    wide_str[length] = L'\0';

    return CResult_create(wide_str, free);
}

#endif // __STDC_VERSION__ >= 201112L