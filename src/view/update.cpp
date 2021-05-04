#include "view/view.hpp"

void    View::update(double time, const Grid& grid, size_t current_block) {
    SDL_SetRenderDrawColor(render, 0, 0 ,0 ,0);
    SDL_RenderClear(render);
    
    hud.update();
    if (clicked or double_clicked) {
        int x_grid = (mouse_x * scale_high_dpi - grid_x) / cell_size;
        int y_grid = (mouse_y * scale_high_dpi - grid_y) / cell_size;

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

        texture_rect.w = static_cast<int>(new_w);
        texture_rect.h = static_cast<int>(new_h);
        texture_rect.x = static_cast<int>(x);
        texture_rect.y = static_cast<int>(y);
    }
    if (delta.size() > 0)
        update_entities(grid, time);
    update_pheromons(grid, current_block);
}

void View::update_pheromons(const Grid &grid, size_t current_block)
{
    SDL_SetRenderTarget(render, pheromons_texture);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    for (Cell *cell : grid.map)
    {
        cell->update(current_block);
        Coordinates c = cell->get_location();
        for (Colony *colony : grid.colonies)
        {
            if (disp_pheromons[colony])
            {
                double alpha = cell->get_nest_pheromons(colony);
                rgb color = m[colony];
                draw_cell_rect(c, color.r, color.g, color.b, std::max(0.0, alpha * 255 - 100));
            }
            if (cell->get_sugar_pheromons() > 0)
            {
                std::cout << cell->get_sugar_pheromons() * 255 << std::endl;
                int alpha = cell->get_sugar_pheromons() * 255;
                if (alpha > 255)
                {
                    alpha = 255;
                }
                rgb color = {1.0, 1.0, 1.0};
                draw_cell_rect(c, color.r, color.g, color.b, alpha);
            }
        }
    }

    SDL_SetRenderTarget(render, NULL);
}

void View::update_entities(const Grid &grid, double delta_time)
{
    SDL_SetRenderTarget(render, entities_texture);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    for (Cell *cell : grid.map)
        if (cell->is_nest())
        {
            rgb color = m[cell->get_nest()];
            draw_cell_rect(cell->get_location(), color.r, color.g, color.b, color.a);
        }
        else if (cell->has_sugar())
            draw_cell_rect(cell->get_location(), 0xFF, 0xFF, 0xFF, 0xFF);

    for (Action action : delta)
    {
        if (action.type == ActionType::AntMove)
        {
            double x = action.updated[1].x * delta_time + action.updated[0].x * (1 - delta_time);
            double y = action.updated[1].y * delta_time + action.updated[0].y * (1 - delta_time);
            rgb color = m[action.colony];
            draw_cell_circle(x, y, color.r, color.g, color.b, color.a);
        }
        else if (action.type == ActionType::AntDeath)
        {
        }
    }
    SDL_SetRenderTarget(render, NULL);
}

void    View::update_map(std::vector<Action> d) {
    delta = d;
}