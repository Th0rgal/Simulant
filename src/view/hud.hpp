#pragma once

#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>

#include "view/color.hpp"

class Button {
    private:
        SDL_Rect    rect;
        SDL_Texture *texture;
        SDL_Texture *highlight;

    public:
        Button(rgb color);
        Button(rgb color, rgb highlight_color);
};

class HUD {
    private:
        SDL_Window *window;
        SDL_Renderer *render;

    public:
        HUD(SDL_Window *w);
        ~HUD();
};