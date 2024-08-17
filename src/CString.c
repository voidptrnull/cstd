#include <CString.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

CResult* CString_new() {
	CString* string = malloc(sizeof(CString));
	int code = CString_init(string, CSTRING_DEFAULT_ALLOC_SIZE);
    if (code) {
        return CResult_ecreate(CError_create("Initialization of CString returned non-zero exit code.", "CString_new", code));
    }

	return CResult_create(string);
}

int CString_init(CString *string, size_t size) {
	if (string == NULL)
		return CSTRING_NULL_STRING;

    CResult* res = CVector_new(size);
    if (CResult_is_error(res)) {
        CResult_free(res);
        return CSTRING_ALLOC_FAILURE;
    }
	string->characters = CResult_get(res);
	CResult_free(res);
    return CSTRING_SUCCESS;
}

int CString_set(CString *string,const char *str) {
	if (string == NULL || str == NULL)
		return CSTRING_NULL_STRING;

	int code = CString_clear(string);
	if (code)
		return code;

	size_t len = strlen(str);
    CVector_reserve(string->characters, len);
	for (size_t i = 0;i < len;i++) {
		code = CVector_add(string->characters, (void*)(uintptr_t)str[i]);
		if (code != CVECTOR_SUCCESS) {
			return CSTRING_OP_FAILURE;
		}
	}

	return CSTRING_SUCCESS;
}

char CString_at(const CString* string, size_t index) {
    if (string == NULL || string->characters == NULL)
        return CSTRING_NULL_STRING;

    if (index >= string->characters->size)
        return CSTRING_INDEX_OUT_OF_BOUNDS;
    
    return *(char*)CVector_get(string->characters, index);
}

int CString_free(CString* string) {
    if (string == NULL)
        return CSTRING_SUCCESS;
    CString_clear(string);
    free(string);
    return CSTRING_SUCCESS;
}

size_t CString_length(const CString* string) {
	return string->characters->size;
}

int CString_append_c(CString* string, const char* str) {
	if (string == NULL || str == NULL)
		return CSTRING_NULL_STRING;

    size_t append_len = strlen(str);
    size_t current_len = string->characters->size;

    if (CVector_reserve(string->characters, current_len + append_len) != CVECTOR_SUCCESS)
        return CSTRING_ALLOC_FAILURE;
    
    for (size_t i = 0; i < append_len; i++)
        if (CVector_add(string->characters, (void*)(uintptr_t)(str[i])) != CVECTOR_SUCCESS)
            return CSTRING_ALLOC_FAILURE;
    
    return CSTRING_SUCCESS;
}

int CString_append(CString* string, CString* str) {
	if (string == NULL || str == NULL)
		return CSTRING_NULL_STRING;

    size_t append_len = str->characters->size;
    size_t current_len = string->characters->size;

    if (CVector_reserve(string->characters, current_len + append_len) != CVECTOR_SUCCESS)
        return CSTRING_ALLOC_FAILURE;
    
    for (size_t i = 0; i < append_len; i++) {
        CResult* res = CVector_get(str->characters, i);
        if (CResult_is_error(res) || CVector_add(string->characters, CResult_get(res)) != CVECTOR_SUCCESS)
            return CSTRING_ALLOC_FAILURE;
        free(res); // don't delete data, just free the pointer to result.
    }
    return CSTRING_SUCCESS;
}

CResult* CString_copy(const CString* source) {
    if (source == NULL)
        return NULL;

    CString* copy = malloc(sizeof(CString));
    if (copy == NULL)
        return NULL;

    if (source->characters == NULL) 
    	return CResult_create(copy);

    CResult* res = CVector_copy(source->characters);
    if (CResult_is_error(res))
        return res;
    copy->characters = CResult_get(res);
    CResult_free(res);
    return CResult_create(copy);
}

int CString_clear(CString* string) {
    if (string == NULL)
        return CSTRING_NULL_STRING;

    if (string->characters != NULL) {
        int code = CVector_clear(string->characters);
        if (code != CVECTOR_SUCCESS)
            return CSTRING_OP_FAILURE;
    }

    CResult* res = CString_new(CSTRING_DEFAULT_ALLOC_SIZE);
    if (CResult_is_error(res)) {
        CResult_free(res);
        return CSTRING_ALLOC_FAILURE;
    }

    string->characters = CResult_get(res);
    CResult_free(res);
    
    return CSTRING_SUCCESS;
}

CResult* CString_c_str(CString* string) {
    if (string == NULL)
        return CResult_ecreate(CError_create("Got null CString as source, unable to convert it C-style strings.", "CString_c_str", CSTRING_NULL_STRING));
    
    if (string->characters->size <= 0 || string->characters == NULL)
        return CResult_create("");

    char* str = malloc(sizeof(char*) * string->characters->size);
    if (str == NULL)
        return CResult_ecreate(CError_create("Unable to allocate memory for C string.", "CString_c_str", CSTRING_ALLOC_FAILURE));
    
    for (size_t i = 0;i < string->characters->size;i++) {
        str[i] = *(char*)CVector_get(string->characters, i);
    }

    return CResult_create((void*)str);
}

int CString_equals(CString* str1, CString* str2) {
    if (str1 == str2) return 1;
    if (str1 == NULL || str2 == NULL) return 0;

    if (str1->characters->size != str2->characters->size) return 0;
    
    for (size_t i = 0;i < str1->characters->size;i++) {
        CResult* res1 = CVector_get(str1->characters, i);
        CResult* res2 = CVector_get(str2->characters, i);
        
        char x,y;
        if (CResult_is_error(res1)) return 0;
        x = *(char*)CResult_get(res1);

        if (CResult_is_error(res2)) return 0;
        y = *(char*)CResult_get(res2);

        if (x != y) return 0;
        free(res1);
        free(res2);
    }

    return 1;
}