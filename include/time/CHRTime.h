/**
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
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef CSTD_CHRTIME_H
#define CSTD_CHRTIME_H

#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
typedef uint64_t hrtime_t;

#define HRTIME_NS() (hrtime_ns())
#define HRTIME_US() (hrtime_us())
#define HRTIME_MS() (hrtime_ms())

inline hrtime_t hrtime_ns() {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (hrtime_t)(counter.QuadPart * 1000000000ULL / frequency.QuadPart);
}

inline hrtime_t hrtime_us() {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (hrtime_t)(counter.QuadPart * 1000000ULL / frequency.QuadPart);
}

inline hrtime_t hrtime_ms() {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (hrtime_t)(counter.QuadPart * 1000ULL / frequency.QuadPart);
}

#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#include <time.h>
typedef uint64_t hrtime_t;

#define HRTIME_NS() (hrtime_ns())
#define HRTIME_US() (hrtime_us())
#define HRTIME_MS() (hrtime_ms())

inline hrtime_t hrtime_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (hrtime_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

inline hrtime_t hrtime_us() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (hrtime_t)ts.tv_sec * 1000000ULL + ts.tv_nsec / 1000ULL;
}

inline hrtime_t hrtime_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (hrtime_t)ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;
}

#else
#error "Unsupported platform"
#endif

#endif // CHRTIME_
