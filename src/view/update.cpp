#include "view/view.hpp"

void    View::renderAll() {
    SDL_SetRenderDrawColor(render, 0, 0 ,0 ,0);
    SDL_RenderClear(render);

    SDL_RenderCopy(render, grid_texture, NULL, NULL);
    SDL_RenderCopy(render, pheromons_texture, NULL, NULL);
    SDL_RenderCopy(render, entities_texture, NULL, NULL);
    SDL_RenderPresent(render);
}

void    View::update(double time, const Grid& grid) {
    SDL_SetRenderDrawColor(render, 0, 0 ,0 ,0);
    SDL_RenderClear(render);
    
    if (clicked) {
        int x_grid = (mouse_x * scale_high_dpi - grid_x) / cell_w;
        int y_grid = (mouse_y * scale_high_dpi - grid_y) / cell_h;

        x_grid += X_MIN;
        y_grid += Y_MIN;

        std::cout << "Cliked at {" << x_grid << ", " << y_grid << "}" << std::endl;

        Cell *cell = grid.get_cell(x_grid, y_grid);
        if (cell and cell->is_nest()) {
            disp_pheromons[cell->get_nest()] = !disp_pheromons[cell->get_nest()];
            update_pheromons(grid);
        }
    }

    //update_entities(grid, time);
}

void    View::update_map(std::vector<Action> d) {
    delta = d;
}