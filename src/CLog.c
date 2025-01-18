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

#include <cstd/CLog.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#define _CLOG_FILE_SEPERATOR '\\'
#else
#define _CLOG_FILE_SEPERATOR '/'
#endif

static int use_colors = 1;
static int use_location = 0;
static int no_debug = 1;
static int short_loc = 0;

#define COLOR_RESET "\033[0m"
#define COLOR_DEBUG "\033[34m"
#define COLOR_INFO "\033[32m"
#define COLOR_WARN "\033[33m"
#define COLOR_ERROR "\033[31m"
#define COLOR_GENERAL "\033[37;1m"

void enable_colors(void) { use_colors = 1; }

void disable_colors(void) { use_colors = 0; }

static void print_timestamp(FILE *stream) {
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    fprintf(stream, "[%02d:%02d:%02d] ", local->tm_hour, local->tm_min,
            local->tm_sec);
}

void CLogImpl(int level, char *_loc, size_t line, const char *format, ...) {
    const char *level_str;
    const char *loc = short_loc && strrchr(_loc, _CLOG_FILE_SEPERATOR)
                          ? strrchr(_loc, _CLOG_FILE_SEPERATOR) + 1
                          : _loc;
    FILE *stream;
    const char *color = COLOR_RESET;

    switch (level) {
    case DEBUG:
        if (no_debug) {
            return;
        }
        level_str = "[DEBUG]";
        stream = stdout;
        color = COLOR_DEBUG;
        break;

    case INFO:
        level_str = "[INFO] ";
        stream = stdout;
        color = COLOR_INFO;
        break;
    case WARN:
        level_str = "[WARN] ";
        stream = stdout;
        color = COLOR_WARN;
        break;
    case ERROR:
        level_str = "[ERROR]";
        stream = stderr;
        color = COLOR_ERROR;
        break;
    default:
        level_str = "[LOG]  ";
        stream = stdout;
        break;
    }

    print_timestamp(stream);
    if (use_colors) {
        fprintf(stream, "%s%s%s ", color, level_str, COLOR_RESET);
    } else {
        fprintf(stream, "%s ", level_str);
    }

    if (use_location) {
        fprintf(stream, "[%s:%zu] ", loc, line);
    }

    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);

    fprintf(stream, "\n");
    fflush(stream);

    if (use_colors) {
        printf(COLOR_RESET);
    }
}

void enable_debugging(void) { no_debug = 0; }

void disable_debugging(void) { no_debug = 1; }

void enable_location(void) { use_location = 1; }

void disable_location(void) { use_location = 0; }

void shortened_location(void) { short_loc ^= 1; }