/**
 * @file ui.c
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief Functions for drawing the CHIP-8 UI.
 * @version 0.1
 * @date 2022-10-06
 *
 * @copyright Copyright (c) 2022 Julianne Adams
 *
 */

#include "ui.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <stdlib.h>

/**
 * @brief Initialize a UI using SDL2.
 *
 * @param width The width of the window displayed by the UI.
 * @param height The height of the window displayed by the UI.
 *
 * @return struct ui* Pointer to the new UI struct.
 */
struct ui* ui_init(unsigned int width, unsigned int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL. SDL error: %s\n", SDL_GetError());
        return NULL;
    }

    struct ui* ui = malloc(sizeof(*ui));
    ui->window = SDL_CreateWindow("LeftySolara's CHIP-8 Interpreter", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!ui->window) {
        printf("Could not create window. SDL error: %s\n", SDL_GetError());
        free(ui);
        return NULL;
    }

    return ui;
}

/**
 * @brief Destroy all windows and deallocate memory from UI.
 *
 * @param ui The UI to destroy.
 */
void ui_free(struct ui* ui)
{
    SDL_DestroyWindow(ui->window);
    free(ui);
    SDL_Quit();
}