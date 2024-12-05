#include <assert.h>
#include <cstd/CLog.h>
#include <stdlib.h>
#include <string.h>
#include <cstd/CString.h>

#define TEST_STRING "THIS IS A TEST STRING"

int test_empty() {
    CLog(INFO, "test_empty()");
    CResult_t *res = CString_new();
    assert(!CResult_is_error(res));
    CString_t *str = CResult_get(res);
    CResult_free(&res);
    assert(CString_length(str) == 0);
    assert(CString_set(str, "") == CSTRING_SUCCESS);
    res = CString_c_str(str);
    assert(!CResult_is_error(res));
    char *s = CResult_get(res);
    assert(strcmp(s, "") == 0);
    CString_free(&str);
    CResult_free(&res);
    return 0;
}

int test_at() {
    CLog(INFO, "test_at()");
    CResult_t *res = CString_new();
    assert(!CResult_is_error(res));
    CString_t *str = CResult_get(res);
    // CResult_free(&res);
    assert(CString_append_c(str, TEST_STRING) == CSTRING_SUCCESS);
    CLog(DEBUG, "Size of string: %llu", CString_length(str));
    CResult_t *res1 = CString_c_str(str);
    assert(!CResult_is_error(res1));
    char *s = CResult_get(res1);
    CLog(DEBUG, "String: %s", s);
    assert(strcmp(s, TEST_STRING) == 0);
    for (int64_t i = 0; i < CString_length(str); i++) {
        CLog(DEBUG, "%c", TEST_STRING[i]);
        CLog(DEBUG, "%c", CString_at(str, i));
        assert(CString_at(str, i) == s[i]);
    }
    CString_free(&str);
    CResult_free(&res1);
    CResult_free(&res);
    return 0;
}

int main() {
    //enable_debugging();
    enable_location();
    shortened_location();
    assert(!test_empty());
    assert(!test_at());
    return 0;
}