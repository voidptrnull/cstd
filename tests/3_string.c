#include <assert.h>
#include <logger/CLog.h>
#include <stdlib.h>
#include <string.h>
#include <util/CString.h>

#define TEST_STRING "THIS IS A TEST STRING"

int test_empty() {
    CLog(INFO, "test_empty()");
    CResult *res = CString_new();
    assert(!CResult_is_error(res));
    CString *str = CResult_get(res);
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
    CResult *res = CString_new();
    assert(!CResult_is_error(res));
    CString *str = CResult_get(res);
    // CResult_free(&res);
    assert(CString_append_c(str, TEST_STRING) == CSTRING_SUCCESS);
    CLog(DEBUG, "Size of string: %llu", CString_length(str));
    CResult *res1 = CString_c_str(str);
    assert(!CResult_is_error(res1));
    char *s = CResult_get(res1);
    CLog(DEBUG, "String: %s", s);
    assert(strcmp(s, TEST_STRING) == 0);
    for (size_t i = 0; i < CString_length(str); i++) {
        CLog(INFO, "%c", TEST_STRING[i]);
        CLog(INFO, "%c", CString_at(str, i));
        assert(CString_at(str, i) == s[i]);
    }
    CString_free(&str);
    CResult_free(&res1);
    CResult_free(&res);
    return 0;
}

int main() {
    enable_debugging();
    assert(!test_empty());
    assert(!test_at());
    return 0;
}