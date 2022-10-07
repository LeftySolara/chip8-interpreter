#include <SDL2/SDL_events.h>

#include "chip8.h"
#include "ui.h"

int main(int argc, char **argv)
{
    struct chip8 *chip8 = chip8_init();

    if (!chip8_load_program(chip8, argv[1])) {
        return 1;
    }

    struct ui *ui = ui_init(SCREEN_WIDTH, SCREEN_HEIGHT);

    int quit = 0;
    while (!quit) {
        chip8_cycle(chip8);
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
