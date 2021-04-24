#include "view/view.hpp"

void    View::update(double time, const Grid& grid) {
    if (clicked) {
        int x_grid = (mouse_x * scale_high_dpi - grid_x) / cell_w + X_MIN;
        int y_grid = (mouse_y * scale_high_dpi - grid_y) / cell_h + Y_MIN;

        x_grid = x_grid < 0 ? x_grid - 1 : x_grid;
        y_grid = y_grid < 0 ? y_grid - 1 : y_grid;

        Cell *cell = grid.get_cell(x_grid, y_grid);
        std::cout << x_grid << ", " << y_grid << std::endl;
        if (cell->is_nest()) {
            disp_pheromons[cell->get_nest()] = !disp_pheromons[cell->get_nest()];
            show_map(grid);
        }
    }
}

void    View::update_map(std::vector<Action> d) {
    delta = d;
}