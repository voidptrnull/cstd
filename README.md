# CSTD

## Purpose

The purpose of CSTD is to provide the standard C library and provide general implementations of widely or commonly used features
like that in other languages.

## Usage & Fitness

I have written this library merely to ease-up developement when writing software with C.
You may use CSTD for any purpose you may deem fit.

While CSTD does not provide emphasis on performance, it tries to be as performant as possible while being able to used.
This does not mean that CSTD will never be a performance "bottle-neck" for your application. If you are encountering such issues,
you are better off writing your own "performant" versions for your use-case. However, if you do think the library can be improved,
please do not hesitate to create a pull request.

## What does it and does not intend to do ?

CSTD intends to do the following:
- Provide common implementations (like dynamic arrays).
- Provide ease-of use utilities (like custom CStrings).
- Provide a general way to perform common system coroutines or tasks (like creating networking sockets).

CSTD does not intend to do the following:
- Provide things that are already in the standard C library (like memory allocators, etc.)

## Usage Philosophy

To use any of the data structure provided in the implementation, the following functions are used:
- `new` - for creating a dynamically allocated 