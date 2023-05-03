//
// Created by galdu on 03/05/2023.
//

#include "stack.h"
#include "grafo_datastruct.h"

Stack *stack_init() {
    Stack *new_stack = (Stack *) malloc(sizeof(Stack));
    int *data_structure = (int *) malloc(sizeof(int) * tamanho);

    for (int i = 0; i < tamanho; i++) data_structure[i] = -1;

    new_stack->size = tamanho;
    new_stack->top = -1;
    new_stack->data = data_structure;

    return new_stack;
}

bool stack_push(Stack *stack, int value) {
    if (stack->top == (tamanho - 1))
        return false;
    else {
        stack->top++;
        stack->data[stack->top] = value;
        return true;
    }
}

int stack_pop(Stack *stack) {
    int top = stack->top;
    int value = stack->data[top];
    stack->data[top] = -1;
    stack->top--;
    return value;
}

bool stack_is_empty(Stack *stack) {
    if (stack->top == -1)
        return true;
    else return false;
}

bool stack_is_full(Stack *stack) {
    if (stack->top == (tamanho - 1))
        return true;
    else return false;
}