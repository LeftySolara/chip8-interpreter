/**
 * @file stack.h
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief A simple stack implementation
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022 Julianne Adams
 *
 */

#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdint.h>

struct stack {
    uint16_t *items;
    size_t size;
    size_t capacity;
};

/** Initialize a stack. */
struct stack *stack_init(size_t capacity);

/** Free memory used by a stack. */
void stack_free(struct stack *stack);

/** Add an item to the top of the stack. */
int stack_push(struct stack *stack, uint16_t data);

/** Remove the top item from a stack and return it. */
uint16_t stack_pop(struct stack *stack);

/** Return the top item of a stack without removing it. */
uint16_t stack_peek(struct stack *stack);

/** Print the contents of a stack. For debugging purposes. */
void stack_print(struct stack *stack);

#endif /* STACK_H */
