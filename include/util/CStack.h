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
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef CSTD_CSTACK_H
#define CSTD_CSTACK_H

#include "CResult.h"

#define CSTACK_NULL_STACK -3
#define CSTACK_OVERFLOW -2
#define CSTACK_ALLOC_FAILURE -1
#define CSTACK_SUCCESS 0

/// \struct CStack
/// \brief Structure for representing the stack.
typedef struct CStack CStack;

CResult *CStack_new();
int CStack_init(CStack *stack);
int CStack_push(CStack *stack, void *item);
CResult *CStack_pop(CStack *stack);
int CStack_clear(CStack *stack);
int CStack_free(CStack **stack);

#endif
