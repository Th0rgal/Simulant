#include "view/view.hpp"

void    View::renderAll() {
    SDL_SetRenderTarget(render, NULL);

    SDL_SetRenderDrawColor(render, 15, 17, 34, 0xFF);
    SDL_RenderClear(render);

    SDL_RenderCopy(render, background_texture, NULL, NULL);
    SDL_RenderCopy(render, pheromons_texture, NULL, NULL);
    SDL_RenderCopy(render, entities_texture, NULL, NULL);
    
    //SDL_RenderCopy(render, pheromons_texture, &texture_rect, NULL);
    //SDL_RenderCopy(render, entities_texture, &texture_rect, NULL);

    hud.render_menus();
    
    SDL_RenderPresent(render);
}


void View::draw_cell_rect(double x_rect, double y_rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    int x = (x_rect - X_MIN + x_shift) * cell_w;
    int y = (y_rect - Y_MIN + y_shift) * cell_h;
    int w = cell_w;
    int h = cell_h;
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(render, r, g, b, a);
    SDL_RenderFillRect(render, &rect);
}

void View::draw_cell_rect(const Coordinates &c, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    draw_cell_rect(c.x, c.y, r, g, b, a);
}


void View::draw_cell_circle(double x_rect, double y_rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a, double scale)
{
    SDL_RendererInfo info;
    SDL_GetRendererInfo(render, &info);

    SDL_SetRenderDrawColor(render, r, g, b, a);
        if (info.flags & SDL_RENDERER_ACCELERATED) {
        int x = (x_rect - X_MIN + x_shift) * cell_w + cell_w / 2;
        int y = (y_rect - Y_MIN + y_shift) * cell_h + cell_h / 2;
        int rayon = cell_w * scale;
        SDL_RenderFillCircle(render, x, y, rayon);
    } else {
        int w = cell_w * scale * 2;
        int h = cell_h * scale * 2;
        int x = (x_rect - X_MIN + x_shift) * cell_w + (cell_w - w) / 2.0;
        int y = (y_rect - Y_MIN + y_shift) * cell_h + (cell_h - h) / 2.0;

        SDL_Rect rect = {x, y, w, h};
        SDL_RenderFillRect(render, &rect);
    }
}

void View::draw_cell_circle(const Coordinates &c, uint8_t r, uint8_t g, uint8_t b, uint8_t a, double scale) {
    draw_cell_circle(c.x, c.y, r, g, b, a, scale);
}