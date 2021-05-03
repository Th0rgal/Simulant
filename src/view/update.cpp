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

void    View::update(double time, const Grid& grid, size_t current_block) {
    SDL_SetRenderDrawColor(render, 0, 0 ,0 ,0);
    SDL_RenderClear(render);
    
    hud.update();
    if (clicked or double_clicked) {
        int x_grid = (mouse_x * scale_high_dpi - grid_x) / cell_w;
        int y_grid = (mouse_y * scale_high_dpi - grid_y) / cell_h;

        x_grid += X_MIN;
        y_grid += Y_MIN;
        Cell *cell = grid.get_cell(x_grid, y_grid);
        if (cell and cell->is_nest()) {
            disp_pheromons[cell->get_nest()] = !disp_pheromons[cell->get_nest()];
        }
        hud.use();
    }
    if (double_clicked) {
    }
    if (scroll != 0) {
        if (scroll == 1) {
            new_w = new_w * 0.95;
            new_h = new_h * 0.95;
        } else {
            new_w = new_w * 1.05;
            new_h = new_h * 1.05;
        }
        double alpha_x = (mouse_x - dest_rect.x) / (double)dest_rect.w;
        x = alpha_x * new_w;

        double alpha_y = (mouse_y - y) / (double)texture_rect.h;
        y = alpha_y * new_h;
        //double new_x = x + alpha_x * texture_rect.w;
        //double d_x = new_x - x;
        //double new_dx = (d_x * new_w) / texture_rect.w;


        // double alpha_y = mouse_y * scale_high_dpi / (double)window_h;
        // double new_y = y + alpha_y * texture_rect.h;
        // double d_y = new_y - y;
        // double new_dy = (d_y * new_h) / texture_rect.h;
        // y = new_y - new_dy;

        std::cout << x << ", " << y << std::endl;

        texture_rect.w = static_cast<int>(new_w);
        texture_rect.h = static_cast<int>(new_h);
        texture_rect.x = static_cast<int>(x);
        texture_rect.y = static_cast<int>(y);
    }
    if (delta.size() > 0)
        update_entities(grid, time);
    update_pheromons(grid, current_block);
}

void    View::update_map(std::vector<Action> d) {
    delta = d;
}