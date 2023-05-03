//
// Created by galdu on 03/05/2023.
//

#ifndef COUNTLOOPS_STACK_H
#define COUNTLOOPS_STACK_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
    int top;
} Stack;

Stack *stack_init();

bool stack_push(Stack *stack, int value);

int stack_pop(Stack *stack);

bool stack_is_empty(Stack *stack);

#endif //COUNTLOOPS_STACK_H
