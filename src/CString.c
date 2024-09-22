#include <debug/debug.h>
#include <stdlib.h>
#include <string.h>
#include <util/CString.h>

struct CString {
    CVector *characters; ///< Vector storing the characters of the string.
};

void freeChar(void *data) { free(data); }

CResult *CString_new() {
    CString *string = malloc(sizeof(CString));

    debug("Making a CString_init call in CString_new.");
    int code = CString_init(string, CSTRING_DEFAULT_ALLOC_SIZE);
    if (code) {
        return CResult_ecreate(CError_create(
            "Initialization of CString returned non-zero exit code.",
            "CString_new", code));
    }

    return CResult_create(string);
}

int CString_init(CString *string, size_t size) {
    if (string == NULL)
        return CSTRING_NULL_STRING;

    debug("Making a CVector_new call in CString_init.");
    debug_i(size);
    CResult *res = CVector_new(size, freeChar);
    if (CResult_is_error(res)) {
        debug("Result is error (in CString_init;from CString_new).");
        CResult_free(&res);
        string->characters = NULL;
        return CSTRING_ALLOC_FAILURE;
    }
    debug("Result is good (in CString_init;from CString_new).");
    string->characters = CResult_get(res);
    CResult_free(&res);
    return CSTRING_SUCCESS;
}

int CString_set(CString *string, const char *str) {
    if (string == NULL || str == NULL)
        return CSTRING_NULL_STRING;

    int code = CString_clear(string);
    if (code)
        return code;

    size_t len = strlen(str);
    debug("Making a CString_init call in CString_set.");
    code = CString_init(string, len);
    if (code)
        return code;
    for (size_t i = 0; i < len; i++) {
        char *ch = malloc(sizeof(char));
        *ch = str[i];
        code = CVector_add(string->characters, ch);
        if (code != CVECTOR_SUCCESS) {
            return CSTRING_OP_FAILURE;
        }
    }

    return CSTRING_SUCCESS;
}

char CString_at(const CString *string, size_t index) {
    if (string == NULL || string->characters == NULL)
        return CSTRING_NULL_STRING;

    if (index >= CVector_size(string->characters))
        return CSTRING_INDEX_OUT_OF_BOUNDS;

    return *(char *)CVector_get(string->characters, index);
}

int CString_free(CString **string) {
    if (string == NULL || *string == NULL)
        return CSTRING_SUCCESS;
    int code = CString_clear(*string);
    if (code)
        return code;
    free(*string);
    *string = NULL;
    return CSTRING_SUCCESS;
}

size_t CString_length(const CString *string) {
    return CVector_size(string->characters);
}

int CString_append_c(CString *string, const char *str) {
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

int CString_append(CString *string, CString *str) {
    if (string == NULL || str == NULL)
        return CSTRING_NULL_STRING;

    size_t append_len = CVector_size(str->characters);
    size_t current_len = CVector_size(string->characters);

    if (CVector_reserve(string->characters, current_len + append_len) !=
        CVECTOR_SUCCESS)
        return CSTRING_ALLOC_FAILURE;

    for (size_t i = 0; i < append_len; i++) {
        CResult *res = CVector_get(str->characters, i);
        if (CResult_is_error(res) ||
            CVector_add(string->characters, CResult_get(res)) !=
                CVECTOR_SUCCESS)
            return CSTRING_ALLOC_FAILURE;
        free(res); // don't delete data, just free the pointer to result.
    }
    return CSTRING_SUCCESS;
}

CResult *CString_copy(const CString *source) {
    if (source == NULL)
        return NULL;

    CString *copy = malloc(sizeof(CString));
    if (copy == NULL)
        return NULL;

    if (source->characters == NULL)
        return CResult_create(copy);

    CResult *res = CVector_copy(source->characters);
    if (CResult_is_error(res))
        return res;
    copy->characters = CResult_get(res);
    CResult_free(&res);
    return CResult_create(copy);
}

int CString_clear(CString *string) {
    if (string == NULL)
        return CSTRING_NULL_STRING;

    if (string->characters != NULL) {
        int code = CVector_clear(string->characters);
        if (code != CVECTOR_SUCCESS)
            return CSTRING_OP_FAILURE;
    }

    CResult *res = CString_new(CSTRING_DEFAULT_ALLOC_SIZE);
    if (CResult_is_error(res)) {
        CResult_free(&res);
        return CSTRING_ALLOC_FAILURE;
    }

    string->characters = CResult_get(res);
    CResult_free(&res);

    return CSTRING_SUCCESS;
}

CResult *CString_c_str(CString *string) {
    if (string == NULL)
        return CResult_ecreate(
            CError_create("Got null CString as source, unable to convert it to "
                          "C-style strings.",
                          "CString_c_str", CSTRING_NULL_STRING));

    if (CVector_size(string->characters) <= 0 || string->characters == NULL)
        return CResult_create("");

    char *str = malloc(sizeof(char) * (CVector_size(string->characters) + 1));
    if (str == NULL)
        return CResult_ecreate(
            CError_create("Unable to allocate memory for C string.",
                          "CString_c_str", CSTRING_ALLOC_FAILURE));

    for (size_t i = 0; i < CVector_size(string->characters); i++) {
        str[i] = *(char *)CVector_get(string->characters, i);
    }
    str[CVector_size(string->characters)] = '\0'; // Null terminator

    return CResult_create((void *)str);
}

int CString_equals(CString *str1, CString *str2) {
    if (str1 == str2)
        return 1;
    if (str1 == NULL || str2 == NULL)
        return 0;

    if (CVector_size(str1->characters) != CVector_size(str2->characters))
        return 0;

    for (size_t i = 0; i < CVector_size(str1->characters); i++) {
        CResult *res1 = CVector_get(str1->characters, i);
        CResult *res2 = CVector_get(str2->characters, i);

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

int64_t CString_compare(CString *str1, CString *str2) {
    if (str1 == str2)
        return 0;
    if (str1 == NULL || str2 == NULL)
        return -0x8000000000000000L;

    if (CString_length(str1) - CString_length(str2))
        return CString_length(str1) - CString_length(str2);

    for (unsigned long int i = 0; i < CString_length(str1); i++) {
        if (CString_at(str1, i) != CString_at(str2, i)) {
            return CString_at(str1, i) - CString_at(str2, i);
        }
    }

    return 0;
}

CResult *CString_substring(const CString *string, size_t start, size_t end) {
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
    CResult *res = CVector_new(substring_length, freeChar);
    if (CResult_is_error(res)) {
        CResult_free(&res);
        return CResult_ecreate(
            CError_create("An error was encountered during creating a new "
                          "vector for storing the substring.",
                          "CString_substring", CSTRING_ALLOC_FAILURE));
    }
    CVector *substring_vector = CResult_get(res);
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

    CString *substring = malloc(sizeof(CString));
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
    return CResult_create(substring);
}
