#include <debug/debug.h>
#include <stdio.h>

void debug(const char *string) {
#ifndef NDEBUG
    printf("%s\n", string);
#endif
}

void debug_i(const long signed int x) {
#ifndef NDEBUG
    printf("%ld\n", x);
#endif
}