/**
 * @file ui.h
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief Functions for displaying the CHIP-8 UI
 * @version 0.1
 * @date 2022-10-07
 *
 * @copyright Copyright (c) 2022 Julianne Adams
 *
 */

#ifndef UI_H
#define UI_H

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>

struct ui {
    SDL_Window *window;
    SDL_Renderer *renderer;

    const int screen_width;
    const int screen_height;

    SDL_Event event;
};

/** Initialize the UI. */
struct ui *ui_init();

/** Close down the UI. */
void ui_free(struct ui *ui);

void ui_draw(struct ui *ui, uint8_t gfx[SCREEN_WIDTH][SCREEN_HEIGHT]);

#endif /* UI_H */