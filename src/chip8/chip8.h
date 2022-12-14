/**
 * @file chip8.h
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief Implementation of CHIP-8 hardware and processes
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022 Julianne Adams
 *
 */

#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#include "stack/stack.h"

#define MEMORY_SIZE 4096
#define STACK_SIZE 16

#define FONT_START_LOCATION 0x050
#define FONT_END_LOCATION 0x09F

#define PROGRAM_START_LOCATION 0x200
#define PROGRAM_END_LOCATION 0xE8F

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

/** General-purpose registers. */
enum gp_register { V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE, VF, REGISTER_COUNT };

struct chip8 {
    uint8_t screen[SCREEN_WIDTH][SCREEN_HEIGHT];

    uint8_t RAM[MEMORY_SIZE];
    struct stack *stack;

    unsigned int PC; /** Program Counter */
    uint16_t index_register;
    uint8_t gp_registers[REGISTER_COUNT];

    uint8_t delay_timer;
    uint8_t sound_timer;
};

struct chip8 *chip8_init();
void chip8_free(struct chip8 *chip8);

void chip8_print_memory(struct chip8 *chip8);

void chip8_load_program(struct chip8 *chip8, const char *path);

/** Fetch the next instruction from memory. */
uint16_t chip8_fetch_instruction(struct chip8 *chip8);

/** Execute the given instruction. */
void chip8_execute_instruction(struct chip8 *chip8, uint16_t instruction);

#endif /* CHIP8_H */
