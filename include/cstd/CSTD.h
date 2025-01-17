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

/// \file CSTD.h
/// \brief This file provides an all-in-one access to all the library
/// co-routines provided in the CSTD implementation.
///
/// \note Including this header file may lead to performance issues:
///     - It violates the "Include What You Use" (IWYU) principle.
///     - Including this file multiple times (without a precompiled header) can
///     cause unnecessary slowdowns due to the repeated compilation of included
///     files.
#ifndef CSTD_H
#define CSTD_H

#define CSTD_VERSION 102202501UL

#include "CError.h"
#include "CHashMap.h"
#include "CHashSet.h"
#include "CLinkedList.h"
#include "CLog.h"
#include "CQueue.h"
#include "CResult.h"
#include "CStack.h"
#include "CString.h"
#include "CVector.h"
#include "Operators.h"

#endif // CSTD_H