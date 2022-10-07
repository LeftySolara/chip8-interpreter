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

#include "chip8.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/utils.h"

#define FONT_START_LOCATION 0x050
#define FONT_END_LOCATION 0x09F
#define PROGRAM_START_LOCATION 0x200
#define PROGRAM_END_LOCATION 0xE8F

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
    for (int i = FONT_START_LOCATION; i <= FONT_END_LOCATION; ++i) {
        chip8->RAM[i] = font_sprites[i - FONT_START_LOCATION];
    }

    return chip8;
}

void chip8_free(struct chip8 *chip8)
{
    free(chip8);
}

/**
 * @brief Load a program into the CHIP-8 memory.
 *
 * @param chip8 The chip8 struct to save the program into.
 * @param path The file path of the program to load.
 *
 * @return 1 if the program was successfully loaded, or 0 otherwise.
 */
int chip8_load_program(struct chip8 *chip8, const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        printf("ERROR: Unable to load program file.\n");
        return 0;
    }

    /* Check that the program will fit in memory. */
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    if (file_size > MEMORY_SIZE - PROGRAM_START_LOCATION) {
        printf("ERROR: Program file too large.\n");
        fclose(fp);
        return 0;
    }

    /* Load the program. */
    uint8_t *loaded_program = chip8->RAM + PROGRAM_START_LOCATION;
    fseek(fp, 0, SEEK_SET);
    fread(loaded_program, sizeof(chip8->RAM), 2, fp);

    /* Reverse each pair of bytes if necessary. */
    if (is_little_endian()) {
        uint8_t temp;
        for (int i = PROGRAM_START_LOCATION; i <= PROGRAM_END_LOCATION; i = i + 2) {
            temp = chip8->RAM[i];
            chip8->RAM[i] = chip8->RAM[i + 1];
            chip8->RAM[i + 1] = temp;
        }
    }

    fclose(fp);
    return 1;
}

void print_memory(struct chip8 *chip)
{
    for (int i = 0; i < MEMORY_SIZE; ++i) {
        printf("%02x ", chip->RAM[i]);
    }
    printf("\n");
}