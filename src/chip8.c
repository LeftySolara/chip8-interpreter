/**
 * @file chip8.c
 * @author Julianne Adams
 * @brief A CHIP-8 interpreter
 * @version 0.1
 * @date 2022-10-06
 *
 * @copyright Copyright (c) 2022 Julianne Adams
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "chip8.h"

uint8_t font_sprites[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
    0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
    0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
    0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
    0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
    0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
    0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
    0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
    0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
    0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
    0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
    0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
    0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
    0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
    0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
    0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
};

/**
 * @brief Initialize a chip8 struct.
 */
struct chip8 *chip8_init()
{
    struct chip8 *chip8 = malloc(sizeof(*chip8));

    /* Clear the memory, display, and stack. */
    memset(chip8->RAM, 0, MEMORY_SIZE);
    memset(chip8->stack, 0, STACK_SIZE * 2);
    for (int i = 0; i < SCREEN_WIDTH; ++i) {
        memset(chip8->screen, 0, SCREEN_HEIGHT);
    }

    chip8->pc = 0x200;
    chip8->sp = 0;
    chip8->I = 0;
    chip8->opcode = 0;

    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    /* Load font */
    for (int i = 0; i < 80; ++i) {
        chip8->RAM[i] = font_sprites[i];
    }

    return chip8;
}

void chip8_free(struct chip8 *chip8)
{
    free(chip8);
}
