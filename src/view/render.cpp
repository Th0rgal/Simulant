#include "view/view.hpp"

void    View::renderAll() {
    SDL_SetRenderTarget(render, NULL);

    SDL_SetRenderDrawColor(render, 15, 17, 34, 0xFF);
    SDL_RenderClear(render);


    int w, h;

    SDL_GetRendererOutputSize(render, &w, &h);

    SDL_RenderCopy(render, background_texture, &texture_rect, &dest_rect);
    SDL_RenderCopy(render, pheromons_texture, &texture_rect, &dest_rect);
    SDL_RenderCopy(render, entities_texture, &texture_rect, &dest_rect);
    
    //SDL_RenderCopy(render, pheromons_texture, &texture_rect, NULL);
    //SDL_RenderCopy(render, entities_texture, &texture_rect, NULL);

    hud.render_menus();
    
    SDL_RenderPresent(render);
}

void View::draw_cell_rect(const Coordinates &c, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    int x = (c.x - X_MIN) * cell_w;
    int y = (c.y - Y_MIN) * cell_h;
    int w = cell_w;
    int h = cell_h;
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(render, r, g, b, a);
    SDL_RenderFillRect(render, &rect);
}

void View::draw_cell_rect(double x_rect, double y_rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    int x = (x_rect - X_MIN) * cell_w;
    int y = (y_rect - Y_MIN) * cell_h;
    int w = cell_w;
    int h = cell_h;
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(render, r, g, b, a);
    SDL_RenderFillRect(render, &rect);
}

void View::draw_cell_circle(const Coordinates &c, uint8_t r, uint8_t g, uint8_t b, uint8_t a, double scale)
{
    int x = (c.x - X_MIN) * cell_w + cell_w / 2;
    int y = (c.y - Y_MIN) * cell_h + cell_h / 2;
    int rayon = cell_w * scale;

    //void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
    //std::cout << a << std::endl;
    SDL_SetRenderDrawColor(render, r, g, b, a);
    SDL_RenderFillCircle(render, x, y, rayon);
    //SDL_RenderFillRect(render, &rect);
}

void View::draw_cell_circle(double x_rect, double y_rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a, double scale)
{
    int x = (x_rect - X_MIN) * cell_w + cell_w / 2;
    int y = (y_rect - Y_MIN) * cell_h + cell_h / 2;
    int rayon = cell_w * scale;

    //void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
    //std::cout << a << std::endl;
    SDL_SetRenderDrawColor(render, r, g, b, a);
    SDL_RenderFillCircle(render, x, y, rayon);
    //SDL_RenderFillRect(render, &rect);
}