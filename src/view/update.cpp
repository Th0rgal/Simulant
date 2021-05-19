#include "view/view.hpp"

void View::update(double time, const Grid &grid, size_t current_block)
{
    x_start = std::max(static_cast<int>(std::floor(-x_shift + X_MIN)), X_MIN);
    x_end = std::min(static_cast<int>(x_start + window_w / cell_size), X_MAX);
    y_start = std::max(static_cast<int>(std::floor(-y_shift + Y_MIN)), Y_MIN);
    y_end = std::min(static_cast<int>(y_start + window_h / cell_size), Y_MAX);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    update_grid();

    hud.update();
    if (clicked or double_clicked)
    {
        int x_grid = (mouse_x * scale_high_dpi) / cell_size - x_shift;
        int y_grid = (mouse_y * scale_high_dpi) / cell_size - y_shift;


        x_grid += X_MIN;
        y_grid += Y_MIN;

        // std::cout << "{--- > " << x_grid << ", " << y_grid << " < ---}" << std::endl;
        // std::cout << "{-- > " << x_start << ", " << y_start << " < --}" << std::endl;
        // std::cout << "{- > " << x_end << ", " << y_end << " < -}" << std::endl;
        // std::cout << "{--- > " << new_w << ", " << new_h << " < ---}" << std::endl;

        Cell *cell = grid.get_cell(x_grid, y_grid);
        if (cell and cell->is_nest())
        {
            disp_pheromones[cell->get_nest()] = !disp_pheromones[cell->get_nest()];
        }
        hud.use();
    }
    if (double_clicked)
    {
    }
    if (scroll != 0)
    {
        double x_grid = (mouse_x * scale_high_dpi) / new_w;
        double y_grid = (mouse_y * scale_high_dpi) / new_w;

        if (scroll == 1)
        {
            new_w = new_w * 0.9;
            new_h = new_h * 0.9;
        }
        else
        {
            new_w = new_w * 1.1;
            new_h = new_h * 1.1;
        }
        cell_size = static_cast<int>(new_w);
        cell_w = cell_size;
        cell_h = cell_size;
        double x_grid2 = (mouse_x * scale_high_dpi) / new_w;
        double y_grid2 = (mouse_y * scale_high_dpi) / new_w;

        x_shift -= x_grid - x_grid2;
        y_shift -= y_grid - y_grid2;
    }

    if (delta.size() > 0)
        update_entities(grid, time);
    update_pheromones(grid, current_block);

    update_info(grid);
}

void View::update_pheromones(const Grid &grid, size_t current_block)
{
    SDL_SetRenderTarget(render, pheromones_texture);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    for (Cell *cell : grid.map)
    {
        cell->update(current_block);
        Coordinates c = cell->get_location();
        for (Colony *colony : grid.colonies)
        {
            if (disp_pheromones[colony])
            {
                double alpha = cell->get_nest_pheromones(colony);
                rgb color = m[colony];
                draw_cell_rect(c, color.r, color.g, color.b, std::max(0.0, alpha * 255 - 100));
            }
            if (cell->get_sugar_pheromones() > 0)
            {
                int alpha = cell->get_sugar_pheromones() * 25;
                if (alpha > 255)
                {
                    alpha = 255;
                }
                rgb color = {0.40, 0.50, 0.70};
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

void View::update_map(std::vector<Action> d)
{
    delta = d;
}

void View::update_grid()
{
    //std::cout << x_shift << ", " << y_shift << ", "<< new_w << ", " << grid_w << std::endl;

    SDL_SetRenderTarget(render, background_texture);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 0x2C, 0x3A, 0x47, 0xFF);

    for (int i = y_start; i <= y_end; i++) {
        for (int j = x_start; j <= x_end; j++) {
            draw_cell_rect(j, i, 0x2C, 0x3A, 0x47, 0xFF, false);
        }
    }
    SDL_SetRenderTarget(render, NULL);
}

void View::update_info(const Grid& grid) {
    TTF_Font *font = TTF_OpenFont("ressources/Marianne-Regular.otf", 32);
//    SDL_Surface *surface = TTF_RenderText_Blended(font, "Colonies", color_sdl);

    SDL_SetRenderTarget(render, info_texture);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);


    int y = 0;
    for (Colony *c : grid.colonies) {
        rgb color = m[c];
        SDL_Color color_sdl = {static_cast<Uint8>(color.r), static_cast<Uint8>(color.g), static_cast<Uint8>(color.b), 0xFF};
        std::string text = std::to_string(c->get_nb_ants());
        SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color_sdl);

        SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);

        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);

        SDL_Rect dest = {3, y, w, h};

        SDL_RenderCopy(render, texture, NULL, &dest);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        y += 50;
    }

    SDL_Rect rect = {0 , 0, 40, y};
    SDL_SetRenderDrawColor(render, 0, 0, 0, 100);
    SDL_RenderFillRect(render, &rect);

    SDL_SetRenderTarget(render, NULL);
}