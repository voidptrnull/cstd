# CSTD

## Purpose

The purpose of CSTD is to provide an extension for the standard C library and provide general implementations of widely or commonly used features like that in other languages. The project is not meant to provide a monolithic versions of everything but rather bits and pieces for which standalones don't make sense.

## Usage & Fitness

I have written this library merely to ease-up developement when writing software with C.
You may use CSTD for any purpose you may deem fit. It does not make any specific claims of usability or fitness for a particular purpose.

While CSTD does not provide emphasis on performance, it tries to be as performant as possible while being as general as it can get.
This does not mean that CSTD can and will never be a performance "bottle-neck" for your application. If you are encountering such issues, you are better off writing your own "performant" versions for your use-case. It also includes abstractions for error handling, which can be a overhead and even cause **MAJOR** headaches. However, if you do think the library can be improved, please do not hesitate to create a pull request.

## What does it and does not intend to do ?

CSTD intends to do the following:
- Provide common implementations of data strucutres (like dynamic arrays).
- Provide ease-of use utilities for improving security (like string wrappers).
- Provide a general way to perform common system coroutines or tasks (like buffered file readers).

CSTD does not intend to do the following:
- Provide things that are already in the standard C library (like memory allocators, regex, etc.)

## Usage Philosophy

To use any of the data structure provided in the implementation, the following functions are used:
- `new` - for creating a dynamically allocated memory.
- `init` - for initializing data structures.
- `clear` - for clearing resources. Does not free the structure itself, just its resources.
- `free` - does what `clear` does, and frees the structure too in addition.

In addition, anytime you get a `CResult_t*` as return, you should use
- `CResult_is_error` to check whether it is an error,
- `CResult_get` to get the value in case of success
- `CResult_eget` to get the `CError*` in case of an error,
- and `CResult_free` in both case of success or failure.

### Example:

```c
#include <cstd/CLog.h>
#include <cstd/CVector.h>

int main() {
    enable_debugging();
    CResult_t *r = CVector_new(
        20,
        NULL); // NULL for destructor because they are stack-allocated, do not free them.
    if (CResult_is_error(r)) {
        CResult_free(&r);
        return 1;
    }
    CVector_t *vector = CResult_get(r);
    CResult_free(&r);

    for (size_t i = 0; i < 200; i++) {
        int *j = malloc(sizeof(int));
        if (j == NULL) {
            CVector_free(&vector);
            CLog(ERROR, "Unable to add number %lu.", i);
            return 1;
        }

        *j = i * 3 + 127;
        if (CVector_add(vector, j)) {
            // Error occured
            CVector_free(&vector);
            CLog(ERROR, "Unable to add number %lu.", i);
            return 1;
        }
    }

    for (size_t i = 0; i < CVector_size(vector); i++) {
        CResult_t *res = CVector_get(vector, i);
        if (CResult_is_error(res)) {
            CVector_free(&vector);
            CResult_free(&res);
            CLog(ERROR, "Unable to access number at index %lu.", i);
            return 1;
        }
        CLog(DEBUG, "Value of vector at index %lu is %lu.", i,
             *(int *)CResult_get(res));
        CResult_free(&res);
    }

    CVector_free(&vector); // Every data strucutre has their own free method. They are not freed with CResult_free.
}
```