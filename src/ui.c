/**
 * @file ui.c
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief Functions for displaying the CHIP-8 UI
 * @version 0.1
 * @date 2022-10-07
 *
 * @copyright Copyright (c) 2022 Julianne Adams
 *
 */

#include "ui.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>

/**
 * @brief Initialize the UI.
 *
 * @return struct ui* A pointer to a struct containing the window, renderer, and surface
 */
struct ui *ui_init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Could not initialize SDL. SDL error: %s\n", SDL_GetError());
        return NULL;
    }

    struct ui *ui = malloc(sizeof(*ui));

    *(int *)&ui->screen_width = SCREEN_WIDTH;
    *(int *)&ui->screen_height = SCREEN_HEIGHT;

    SDL_CreateWindowAndRenderer(ui->screen_width, ui->screen_height, 0, &ui->window, &ui->renderer);
    SDL_SetRenderDrawColor(ui->renderer, 73, 100, 122, 100);
    SDL_RenderClear(ui->renderer);
    SDL_RenderPresent(ui->renderer);

    return ui;
}

/**
 * @brief Free memory used by the UI.
 */
void ui_free(struct ui *ui)
{
    SDL_DestroyWindow(ui->window);
    SDL_DestroyRenderer(ui->renderer);
    SDL_Quit();

    free(ui);
}

/**
 * @brief Draw the UI based on the data in gfx.
 */
void ui_draw(struct ui *ui, uint8_t gfx[SCREEN_WIDTH][SCREEN_HEIGHT])
{
    SDL_RenderClear(ui->renderer);
    for (int x = 0; x < ui->screen_width; ++x) {
        for (int y = 0; y < ui->screen_height; ++y) {
            if (gfx[x][y] == 0) {
                SDL_SetRenderDrawColor(ui->renderer, 73, 100, 122, 100);
            }
            else {
                SDL_SetRenderDrawColor(ui->renderer, 197, 200, 198, 100);
            }
            SDL_RenderDrawPoint(ui->renderer, x, y);
        }
    }
    SDL_RenderPresent(ui->renderer);
}