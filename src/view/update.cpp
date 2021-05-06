#include "view/view.hpp"

void    View::update(double time, const Grid& grid, size_t current_block) {
    SDL_SetRenderDrawColor(render, 0, 0 ,0 ,0);
    SDL_RenderClear(render);
    
    update_grid();    

    hud.update();
    if (clicked or double_clicked) {
        int x_grid = (mouse_x * scale_high_dpi) / new_w - x_shift;
        int y_grid = (mouse_y * scale_high_dpi) / new_h - y_shift;

        x_grid += X_MIN;
        y_grid += Y_MIN;

        // std::cout << "{--- > " << x_grid << ", " << y_grid << " < ---}" << std::endl;
        // std::cout << "{--- > " << new_w << ", " << new_h << " < ---}" << std::endl;

        Cell *cell = grid.get_cell(x_grid, y_grid);
        if (cell and cell->is_nest()) {
            disp_pheromons[cell->get_nest()] = !disp_pheromons[cell->get_nest()];
        }
        hud.use();
    }
    if (double_clicked) {
    }
    if (scroll != 0) {
        double x_grid = (mouse_x * scale_high_dpi) / new_w;
        double y_grid = (mouse_y * scale_high_dpi) / new_w;

        if (scroll == 1) {
            new_w = new_w * 0.9;
            new_h = new_h * 0.9;
        } else {
            new_w = new_w * 1.1;
            new_h = new_h * 1.1;
        }
        cell_size = static_cast<int>(new_w);
        cell_w = cell_size * zoom_in;
        cell_h = cell_size * zoom_in;
        double x_grid2 = (mouse_x * scale_high_dpi) / new_w;
        double y_grid2 = (mouse_y * scale_high_dpi) / new_w;

       x_shift -= x_grid - x_grid2;
       y_shift -= y_grid - y_grid2;
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

void    View::update_grid() {
    SDL_SetRenderTarget(render, background_texture);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 0x2C, 0x3A, 0x47, 0xFF);
    for (int i = 0; i < SPACE_HEIGHT; i++)
    {
        for (int j = 0; j < SPACE_WIDTH; j++)
        {
            SDL_Rect rect = {static_cast<int>((j + x_shift) * cell_w), static_cast<int>((i + y_shift) * cell_h), cell_w, cell_h};
            SDL_RenderDrawRect(render, &rect);
        }
    }
    SDL_SetRenderTarget(render, NULL);
}