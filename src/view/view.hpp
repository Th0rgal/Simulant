#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <unordered_set>
#include <map>

#include "models/grid.hpp"

struct rgb{
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
};

struct hsv{
    double h;// angle in degrees
    double s;// a fraction between 0 and 1
    double v;// a fraction between 0 and 1
};

class View {
public:
    View(bool fullScreen = false);
    View(int w, int h);
    ~View();

    void    init_grid();
    void    disp_grid(const Grid  &grid);
    bool    event_manager();
    void    show_grid();
    void    draw_cell(Coordinates& c, uint8_t r, uint8_t g, uint8_t b);


private:
    SDL_Window      *window;
    SDL_Renderer    *render;
    int             window_w;
    int             window_h;
    int             cell_w;
    int             cell_h;
    int             grid_w;
    int             grid_h;
    int             grid_x;
    int             grid_y;

    SDL_Rect        top;
    SDL_Rect        right;
    SDL_Rect        left;
    SDL_Rect        down;
};