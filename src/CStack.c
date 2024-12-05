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

#include <cstd/CStack.h>
#include <stdlib.h>

struct CStackNode {
    void *data;
    struct CStackNode *next;
};

struct _CStack {
    struct CStackNode *top;
    size_t size;
    Destructor destroy;
};

CResult_t *CStack_new(Destructor destroy) {
    CStack_t *stack = malloc(sizeof(CStack_t));
    if (!stack) {
        return CResult_ecreate(
            CError_create("Unable to allocate memory to create a new stack.",
                          "CStack_new()", CSTACK_ALLOC_FAILURE));
    }

    if (CStack_init(stack, destroy) != CSTACK_SUCCESS) {
        free(stack);
        return CResult_ecreate(
            CError_create("Unable to initialize the newly created stack.",
                          "CStack_new()", CSTACK_ALLOC_FAILURE));
    }

    return CResult_create(stack, NULL);
}

int CStack_init(CStack_t *stack, Destructor destroy) {
    if (stack == NULL) {
        return CSTACK_NULL_STACK;
    }

    stack->top = NULL;
    stack->size = 0;
    stack->destroy = destroy;
    return CSTACK_SUCCESS;
}

CResult_t *CStack_pop(CStack_t *stack) {
    if (stack == NULL) {
        return CResult_ecreate(
            CError_create("Received a null pointer as stack.", "CStack_pop",
                          CSTACK_NULL_STACK));
    }

    if (stack->size == 0) {
        return CResult_ecreate(CError_create("Cannot pop from an empty stack.",
                                             "CStack_pop", CSTACK_NULL_STACK));
    }

    struct CStackNode *temp = stack->top;
    void *data = temp->data;
    stack->top = temp->next;
    free(temp);
    stack->size--;

    return CResult_create(data, NULL);
}

int CStack_push(CStack_t *stack, void *item) {
    if (stack == NULL)
        return CSTACK_NULL_STACK;

    struct CStackNode *new_top = malloc(sizeof(struct CStackNode));
    if (new_top == NULL) {
        return CSTACK_ALLOC_FAILURE;
    }

    new_top->data = item;
    new_top->next = stack->top;
    stack->top = new_top;
    stack->size++;

    return CSTACK_SUCCESS;
}

int CStack_clear(CStack_t *stack) {
    if (stack == NULL) {
        return CSTACK_NULL_STACK;
    }

    struct CStackNode *node = stack->top;
    while (node != NULL) {
        struct CStackNode *next = node->next;
        if (stack->destroy)
            stack->destroy(node->data);
        free(node);
        node = next;
    }
    stack->top = NULL;
    stack->size = 0;

    return CSTACK_SUCCESS;
}

int CStack_free(CStack_t **stack) {
    if (stack == NULL || *stack == NULL) {
        return CSTACK_SUCCESS;
    }

    CStack_clear(*stack);
    free(*stack);
    *stack = NULL;
    return CSTACK_SUCCESS;
}
