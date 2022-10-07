#include "chip8.h"

int main(int argc, char **argv)
{
    struct chip8 *chip8 = chip8_init();

    print_memory(chip8);

    chip8_free(chip8);
    return 0;
}
