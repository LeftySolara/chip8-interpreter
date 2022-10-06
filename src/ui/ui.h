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
#include <SDL2/SDL_video.h>

struct ui {
    SDL_Window *window;
    SDL_Event event;
};

/** Initialize a UI */
struct ui *ui_init(unsigned int width, unsigned int height);

void ui_free(struct ui *ui);

#endif /* UI_H */