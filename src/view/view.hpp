#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "models/grid.hpp"

class View {
public:
    View(bool fullScreen = false);
    View(int w, int h);
    ~View();

    void    disp_map(std::array<Cell *, SPACE_WIDTH * SPACE_HEIGHT>  map);
    bool    event_manager();
    void    show_grid();

private:
    SDL_Window *window;
    SDL_Renderer *render;
};