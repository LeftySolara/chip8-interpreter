/**
 * @file ui.h
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief Functions for drawing the CHIP-8 UI.
 * @version 0.1
 * @date 2022-10-06
 *
 * @copyright Copyright (c) 2022 Julianne Adams
 *
 */

#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

struct ui {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
};

/** Initialize a UI */
struct ui *ui_init(unsigned int width, unsigned int height);

/** Free memory used by a UI. */
void ui_free(struct ui *ui);

void ui_draw(struct ui *ui, uint8_t pixels[SCREEN_WIDTH][SCREEN_HEIGHT]);

#endif /* UI_H */