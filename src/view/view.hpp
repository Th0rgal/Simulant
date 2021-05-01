#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <unordered_set>
#include <map>

#include "models/grid.hpp"
#include "models/action.hpp"
#include "view/color.hpp"
#include "view/hud.hpp"

enum struct Event {
    close_request,
    restart,
    none
};

class View {
public:
    View(bool fullScreen = false);
    View(int w, int h);
    ~View();

    void    init_grid();
    void    init_hud();
    Event   event_manager();

    void    clear();

    void    draw_cell_rect(const Coordinates& c, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void    draw_cell_rect(double x_rect, double y_rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    void    draw_cell_circle(const Coordinates& c, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void    draw_cell_circle(double x_rect, double y_rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a);


    void    init_grid(const Grid &grid);
    void    init_entities(const Grid &grid);
    void    update_pheromons(const Grid &grid);
    void    update_entities(const Grid &grid, double delta_time);

    void    update(double time, const Grid &grid);
    void    update_map(std::vector<Action> d);

    void    renderAll();

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

    bool            clicked;
    int             mouse_x;
    int             mouse_y;

    SDL_Texture     *grid_texture;
    SDL_Texture     *entities_texture;
    SDL_Texture     *pheromons_texture;


    std::map<const Colony *, rgb> m;
    std::map<const Colony *, bool> disp_pheromons;

    std::vector<Action> delta;

    bool            end;

    HUD             hud;
};

void    DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);
void    SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);