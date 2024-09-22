#include <stdlib.h>
#include <util/CStack.h>

struct CStackNode {
    void *data;
    struct CStackNode *next;
};

struct CStack {
    struct CStackNode *top;
    unsigned long int size;
};

CResult *CStack_new() {
    CStack *stack = malloc(sizeof(CStack));
    if (!stack) {
        return CResult_ecreate(
            CError_create("Unable to allocate memory to create a new stack.",
                          "CStack_new()", CSTACK_ALLOC_FAILURE));
    }
    int k;
    if ((k = CStack_init(stack))) {
        return CResult_ecreate(
            CError_create("Unable to initialize the newly created stack.",
                          "CStack_new()", k));
    }
    return CResult_create(stack);
}

int CStack_init(CStack *stack) {
    if (stack == NULL) {
        return CSTACK_NULL_STACK;
    }

    stack->top = malloc(sizeof(struct CStackNode));
    if (stack->top == NULL) {
        return CSTACK_ALLOC_FAILURE;
    }
    stack->top->data = NULL;
    stack->top->next = NULL;
    return CSTACK_SUCCESS;
}

CResult *CStack_pop(CStack *stack) {
    if (stack == NULL) {
        return CResult_ecreate(
            CError_create("Recieved a null pointer as stack.", "CStack_pop",
                          CSTACK_NULL_STACK));
    }

    if (stack->top == NULL || stack->size == 0) {
        return CResult_ecreate(
            CError_create("Nothing to pop as top pointer is null.",
                          "CStack_pop", CSTACK_NULL_STACK));
    }

    struct CStackNode *temp = stack->top;
    void *data = temp->data;
    stack->top = stack->top->next;
    free(temp);
    stack->size--;

    return CResult_create(data);
}

int CStack_push(CStack *stack, void *item) {
    if (stack == NULL)
        return CSTACK_NULL_STACK;
    if (stack->top == NULL) {
        // Try to perform an allocation
        stack->top = malloc(sizeof(struct CStackNode));
        // If it is still NULL, return.
        if (stack->top == NULL)
            return CSTACK_NULL_STACK;
    }

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

int CStack_clear(CStack *stack) {
    if (stack == NULL) {
        return CSTACK_NULL_STACK;
    }

    struct CStackNode *node = stack->top;
    while (node != NULL) {
        struct CStackNode *next = node->next;
        free(node);
        node = next;
    }
    stack->top = NULL;
    stack->size = 0;

    return CSTACK_SUCCESS;
}

int CStack_free(CStack **stack) {
    if (stack == NULL || *stack == NULL) {
        return CSTACK_SUCCESS;
    }

    int code = CStack_clear(*stack);
    if (code)
        return code;
    free(*stack);
    *stack = NULL;
    return CSTACK_SUCCESS;
}
