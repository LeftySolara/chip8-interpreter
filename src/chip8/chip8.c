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
    chip8->PC = (uint8_t)PROGRAM_START_LOCATION;

    /* Set registers. */
    chip8->index_register = 0;

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
