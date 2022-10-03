/**
 * @file stack.c
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief A simple stack implementation
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022 Julianne Adams
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

/**
 * @brief Initialize a stack
 *
 * @param size The maximum size of the stack.

 * @return struct stack* A pointer to the newly-allocated stack.
 */
struct stack *stack_init(size_t capacity)
{
    struct stack *stack = malloc(sizeof(*stack));

    /* Memory allocation failure. */
    if (!stack) {
        exit(EXIT_FAILURE);
    }

    stack->items = malloc(sizeof(uint16_t) * capacity);
    memset(stack->items, 0, capacity);

    stack->capacity = capacity;
    stack->size = 0;

    return stack;
}

/** Free memory used by a stack. */
void stack_free(struct stack *stack)
{
    free(stack->items);
    free(stack);
}

/**
 * @brief Add an item to the top of a stack.
 *
 * @param stack The stack to push to.
 * @param data The item to add to the stack.
 *
 * @return int 1 if the data was successfully pushed, or 0 otherwise.
 */
int stack_push(struct stack *stack, uint16_t data)
{
    if (stack->size == stack->capacity) {
        return 0;
    }

    *(stack->items + stack->size++) = data;
    return 1;
}

/**
 * @brief Remove the top item from the stack and return it.
 *
 * @param stack The stack to pop from.
 *
 * @return uint16_t The value at the top of the stack.
 */
uint16_t stack_pop(struct stack *stack)
{
    if (stack->size == 0) {
        return UINT16_MAX;
    }

    uint16_t value = *(stack->items + stack->size - 1);

    *(stack->items + stack->size - 1) = 0;
    --stack->size;

    return value;
}

/**
 * @brief Return the top item of a stack without removing it.
 *
 * @param stack The stack to check.
 *
 * @return uint16_t The value of the item on the top of the stack.
 */
uint16_t stack_peek(struct stack *stack)
{
    return *(stack->items + stack->size - 1);
}

/**
 * @brief Print the contents of a stack.
 *
 * @param stack The stack to print.
 */
void stack_print(struct stack *stack)
{
    for (int i = 0; i < stack->size; ++i) {
        printf("%d ", *(stack->items + i));
    }
    printf("\n");
}
