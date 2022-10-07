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
        memset(chip8->screen[i], 0, SCREEN_HEIGHT);
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

/**
 * @brief Fetch the next opcode in memory.
 *
 * @param chip8 The CHIP-8 interpreter to fetch from.
 *
 * @return struct opcode* The 16-bit opcode for the instruction.
 *
 * This function fetches the next CHIP-8 opcode in memory. The return value
 * is a struct containing the full, 16-bit opcode along with convienence variables
 * representing each nibble of the opcode. Space for this struct is allocated
 * as if malloc() was called, and thus it must be passed to free() to avoid
 * a memory leak.
 */
struct opcode *chip8_fetch_opcode(struct chip8 *chip8)
{
    uint8_t high_byte = chip8->RAM[chip8->pc];
    uint8_t low_byte = chip8->RAM[chip8->pc + 1];

    struct opcode *opcode = malloc(sizeof(*opcode));
    opcode->code = (high_byte << 8) | low_byte;

    opcode->n1 = opcode->code >> 12;
    opcode->n2 = (opcode->code >> 8) & 0x0F;
    opcode->n3 = (opcode->code >> 4) & 0x00F;
    opcode->n4 = opcode->code & 0x000F;

    opcode->NN = opcode->code & 0x00FF;
    opcode->NNN = opcode->code & 0x0FFF;

    chip8->pc += 2;
    return opcode;
}

/**
 * @brief Run one cycle of the fetch, decode, execute loop.
 *
 * @param chip8 The CHIP-8 interpreter to run.
 */
void chip8_cycle(struct chip8 *chip8)
{
    /* Fetch the next opcode in memory.
     *
     * List of opcodes:
     * 0NNN: Calls program at address NNN. Not necessary for most ROMs
     * 00E0: Clears the screen
     * 00EE: Returns from a subroutine
     * 1NNN: Jumps to address NNN
     * 2NNN: Calls subreoutine at address NNN
     * 3XNN: Skips the next instruction if VX equals NN
     * 4XNN: Skips the next instruction if VX is not equal to NN
     * 5XY0: Skips the next instruction if VX equals VY
     * 6XNN: Sets the value of register VX to NN
     * 7XNN: Adds NN to VX (carry flag not changed)
     * 8XY0: Sets VX to the vaule of VY
     * 8XY1: Sets VX to VX OR VY
     * 8XY2: Sets VX to VX AND VY
     * 8XY3: Sets VX to VX XOR VY
     * 8XY4: Sets VX to VX + VY with VF = carry
     * 8XY5: Sets Vx to VX - VY with VF = NOT borrow
     * 8XY6: Sets VX to VX SHR 1
     * 8XY7: Sets VX = VY - VX with VF = NOT borrow
     * 8XYE: Sets VX = SHL 1
     * 9XY0: Skips the next instruction if VX is not equal to VY
     * ANNN: Sets register I to NNN
     * BNNN: Jumps to address NNN + V0
     * CXNN: Sets VX to the result of a bitwise AND on a random number and NN
     * DXYN: Draws an N-byte sprites starting at memory location I at (VX, VY), set VF = collision
     * EX9E: Skips the next instruction if the key stored in VX is pressed
     * EXA1: Skips the next instruction if the key stored in VX is not pressed
     * FX07: Sets VX to the value of the delay timer
     * FX0A: A key press is awaited, and then stored in VX (blocking operation)
     * FX15: Sets the delay timer to VX
     * FX18: Sets the sound timer to VX
     * FX1E: Adds VX to I
     * FX29: Sets I to the location of the sprite for the character in VX
     * FX33: Stores the BCD of VX starting at memory location I
     * FX55: Stores V0 to VX (including VX) in memory starting at address I
     * FX65: Fills V0 to VX (including VX) with values from memory starting at address I
     */
    struct opcode *opcode = chip8_fetch_opcode(chip8);

    /* Decode the curent opcode. */
    ;

    /* Execute the opcode instruction. */
    ;

    free(opcode);
}

void print_memory(struct chip8 *chip)
{
    for (int i = 0; i < MEMORY_SIZE; ++i) {
        printf("%02x ", chip->RAM[i]);
    }
    printf("\n");
}