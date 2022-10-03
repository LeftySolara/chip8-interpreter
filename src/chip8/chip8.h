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

#define MEMORY_SIZE 4096

#define FONT_START_LOCATION 0x050
#define FONT_END_LOCATION 0x09F

#define PROGRAM_START_LOCATION 0x200
#define PROGRAM_END_LOCATION 0xE8F

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

struct chip8 {
    uint8_t RAM[MEMORY_SIZE];
    uint8_t screen[SCREEN_WIDTH][SCREEN_HEIGHT];

    uint8_t PC; /** Program Counter */
    uint16_t index_register;
};

struct chip8 *chip8_init();
void chip8_free(struct chip8 *chip8);

void chip8_print_memory(struct chip8 *chip8);

void chip8_load_program(struct chip8 *chip8, const char *path);

#endif /* CHIP8_H */
