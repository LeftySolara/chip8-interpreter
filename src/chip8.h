/**
 * @file chip8.h
 * @author Julianne Adams
 * @brief A CHIP-8 interpreter
 * @version 0.1
 * @date 2022-10-06
 *
 * @copyright Copyright (c) 2022 Julianne Adams
 *
 */

#ifndef CHIP8_H
#define CHIP8_H

#define MEMORY_SIZE 4096
#define STACK_SIZE 16

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

#include <stdint.h>

struct chip8 {
    /**
     * CPU Registers:
     *
     * The 16 8-bit general purpose registers are referred to as Vx,
     * where x is a hexadecimal digit. The VF register shouldn't be used
     * by programs, as it's used as a flag by some instructions.
     *
     * The register I is the index register. It is mostly used to
     * store memory addresses, so usually just the lowest 12 bits are used.
     */
    uint8_t V[16];
    uint16_t I;

    /**
     * Memory:
     *
     * The CHIP-8 interpreter has 4K of memory. The first 512 bytes
     * (0x000 to 0x1FF) are used by the interpreter and therefore
     * should not be accessed by programs.
     */
    uint8_t RAM[MEMORY_SIZE];

    /**
     * Display:
     *
     * The display operates at 60 Hz.
     */
    uint8_t screen[SCREEN_WIDTH][SCREEN_HEIGHT];

    /** Program Counter: points to current instruction in memory. */
    unsigned int pc;

    /** Stack pointer: points to the top of the stack. */
    unsigned short sp;

    /** Stack: used to call subroutines and return from them. */
    uint16_t stack[STACK_SIZE];

    /**
     * Timers
     *
     * These timers bother decrement at a rate of 60 Hz until they reach 0.
     * The sound timer gives off a beeping sound as long asa it's not 0.
     */
    uint8_t delay_timer;
    uint8_t sound_timer;

    /**
     * Opcode:
     *
     * The opcode of the current instruction in memory.
     */
    uint16_t opcode;
};

struct opcode {
    uint16_t code; /** The full 16-bit opcode */

    uint16_t n1; /** The first nibble */
    uint16_t n2; /** The second nibble */
    uint16_t n3; /** The third nibble */
    uint16_t n4; /** The fourth nibble */

    uint16_t NN;  /** The second byte */
    uint16_t NNN; /** The second, third, and fourth nibble */
};

/** Initialize a chip8 structure. */
struct chip8 *chip8_init();
/** Free memory used by a chip8 structure. */
void chip8_free(struct chip8 *chip8);

/** Load a CHIP-8 program into an interpreter. */
int chip8_load_program(struct chip8 *chip8, const char *path);

/** Fetch the next opcode in memory. */
struct opcode *chip8_fetch_opcode(struct chip8 *chip8);

/** Run one cycle of the fetch, decode, excute loop. */
void chip8_cycle(struct chip8 *chip8);

void print_memory(struct chip8 *chip);

#endif /* CHIP8_H */
