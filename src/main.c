#include <stdint.h>

#include "chip8/chip8.h"
#include "ui/ui.h"

int main(int argc, char **argv)
{
    struct chip8 *chip8 = chip8_init();
    struct ui *ui = ui_init(SCREEN_WIDTH, SCREEN_HEIGHT);

    chip8_load_program(chip8, argv[1]);

    /*
        for (int i = PROGRAM_START_LOCATION; i < PROGRAM_END_LOCATION; i = i + 2) {
            // printf("%04x\n", chip8_fetch_instruction(chip8));
            uint16_t instruction = chip8_fetch_instruction(chip8);
            chip8_execute_instruction(instruction);
        }
    */

    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&ui->event) != 0) {
            if (ui->event.type == SDL_QUIT) {
                quit = 1;
            }
            else if (ui->event.type == SDL_KEYDOWN) {
                switch (ui->event.key.keysym.sym) {
                    case SDLK_q:
                        quit = 1;
                        break;
                    default:
                        break;
                }
            }
        }
        ui_draw(ui, chip8->screen);
    }

    ui_free(ui);
    chip8_free(chip8);

    return 0;
}
