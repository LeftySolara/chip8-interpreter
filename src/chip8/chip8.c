/**
 * @file chip8.c
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief Implementation of CHIP-8 hardware and processes
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022 Julianne Adams
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"
#include "chip8.h"

struct chip8 *chip8_init()
{
    /* Initialize memory. */
    struct chip8 *chip8 = malloc(sizeof(*chip8));
    memset(chip8->RAM, 0, MEMORY_SIZE);

    /* Initialize the stack. */
    chip8->stack = stack_init(STACK_SIZE);

    /* Initialize the screen. */
    for (int i = 0; i < SCREEN_WIDTH; ++i) {
        memset(chip8->screen[i], 0, SCREEN_HEIGHT);
    }

    /* Load the built-in font. */
    uint8_t sprites[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
        0x20, 0x60, 0x20, 0x20, 0x70,  // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
        0xF0, 0x80, 0xF0, 0x80, 0x80   // F
    };

    for (int i = 0; i < FONT_END_LOCATION - FONT_START_LOCATION; ++i) {
        chip8->RAM[FONT_START_LOCATION + i] = sprites[i];
    }

    /* Set the program counter. */
    chip8->PC = PROGRAM_START_LOCATION;

    /* Set registers. */
    chip8->index_register = 0;
    memset(chip8->gp_registers, 0, REGISTER_COUNT);

    /* Set timers. */
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    return chip8;
}

void chip8_free(struct chip8 *chip8)
{
    stack_free(chip8->stack);
    free(chip8);
}

void chip8_print_memory(struct chip8 *chip8)
{
    for (int i = 0; i < MEMORY_SIZE; ++i) {
        printf("%02x ", chip8->RAM[i]);
    }
    printf("\n");
}

void chip8_load_program(struct chip8 *chip8, const char *path)
{
    FILE *fp = fopen(path, "rb");
    uint8_t *loaded_program = chip8->RAM + PROGRAM_START_LOCATION;

    fread(loaded_program, sizeof(chip8->RAM), 2, fp);

    if (is_little_endian()) {
        /* Reverse each pair of bytes. */
        uint8_t temp;
        for (int i = PROGRAM_START_LOCATION; i <= PROGRAM_END_LOCATION; i = i + 2) {
            temp = chip8->RAM[i];
            chip8->RAM[i] = chip8->RAM[i + 1];
            chip8->RAM[i + 1] = temp;
        }
    }

    fclose(fp);
}

/**
 * @brief Fetch the next instruction from memory.
 *
 * @param chip8 The chip8 interpreter instance to pull from.
 *
 * @return uint16_t The two-byte instruction pointed to by the program counter.
 */
uint16_t chip8_fetch_instruction(struct chip8 *chip8)
{
    uint8_t high_byte = chip8->RAM[chip8->PC];
    uint8_t low_byte = chip8->RAM[chip8->PC + 1];

    uint16_t opcode = (high_byte << 8) | low_byte;

    chip8->PC = chip8->PC + 2;
    return opcode;
}

/**
 * @brief Execute the given instruction.
 *
 * List of instructions:
 * 00E0: clears the screen
 *
 * @param instruction The instruction to execute.
 */
void chip8_execute_instruction(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t n1 = instruction >> 12;          /* First nibble */
    uint16_t n2 = (instruction >> 8) & 0x0F;  /* Second nibble. */
    uint16_t n3 = (instruction >> 4) & 0x00F; /* Third nibble. */
    uint16_t n4 = instruction & 0x000F;       /* Fourth nibble. */
    uint16_t NN = instruction & 0x00FF;       /* Second byte. */
    uint16_t NNN = instruction & 0x0FFF;      /* Second, third, and fourth nibbles. */

    switch (n1) {
        case 0:
            switch (NNN) {
                case 0x0E0: /* 00E0 */
                    memset(chip8->screen, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
                    break;
            }
            break;
        default:
            break;
    }
}
