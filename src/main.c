#include "chip8.h"

int main(int argc, char **argv)
{
    struct chip8 *chip8 = chip8_init();

    if (!chip8_load_program(chip8, argv[1])) {
        return 1;
    }

    chip8_cycle(chip8);

    print_memory(chip8);

    chip8_free(chip8);
    return 0;
}
