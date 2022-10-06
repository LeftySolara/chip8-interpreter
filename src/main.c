#include <stdint.h>

#include "chip8/chip8.h"
#include "stdio.h"

int main(int argc, char **argv)
{
    struct chip8 *chip8 = chip8_init();

    chip8_load_program(chip8, argv[1]);

    for (int i = PROGRAM_START_LOCATION; i < PROGRAM_END_LOCATION; i = i + 2) {
        // printf("%04x\n", chip8_fetch_instruction(chip8));
        uint16_t instruction = chip8_fetch_instruction(chip8);
        chip8_execute_instruction(instruction);
    }

    chip8_free(chip8);

    return 0;
}
