#include "view/view.hpp"
#include <algorithm>

double scale_high_dpi = 0;

View::View(bool fullScreen)
{
    int w;
    int h;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    if (!fullScreen)
        window = SDL_CreateWindow("Ant", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_ALLOW_HIGHDPI);
    else
        window = SDL_CreateWindow("Ant", 0, 0, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALLOW_HIGHDPI);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);

    SDL_GL_GetDrawableSize(window, &window_w, &window_h);
    SDL_GetWindowSize(window, &w, &h);

    scale_high_dpi = window_w / (double)w;

    init_grid();
    init_hud();

    end = false;
    restart = false;
}

View::View(int w, int h)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    window = SDL_CreateWindow("Ant", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_ALLOW_HIGHDPI);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
    SDL_GL_GetDrawableSize(window, &window_w, &window_h);

    scale_high_dpi = window_w / (double)w;

    //SDL_Surface *surface = SDL_GetWindowSurface(window); || might be usefull to record the game

    init_grid();
    init_hud();

    end = false;
    restart = false;
}

View::~View()
{
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    m.clear();
    disp_pheromons.clear();
    SDL_DestroyTexture(background_texture);
    SDL_DestroyTexture(entities_texture);
    SDL_DestroyTexture(pheromons_texture);

    SDL_Quit();
}

void View::init_grid()
{
    cell_size = std::min(window_w / SPACE_WIDTH, window_h / SPACE_HEIGHT);

    std::cout << cell_size << std::endl;
    cell_w = cell_size;
    cell_h = cell_size;
    grid_x = 0;
    grid_y = 0;

    grid_w = cell_w * SPACE_WIDTH;
    grid_h = cell_h * SPACE_HEIGHT;
    if (grid_w < window_w)
    {
        grid_x = (window_w - grid_w) / 2;
    }
    if (grid_h < window_h)
    {
        grid_y = (window_h - grid_h) / 2;
    }
    double zoom_in = 2.0 / scale_high_dpi;
    dest_rect = {grid_x, grid_y, grid_w, grid_h};
    grid_w *= zoom_in;
    grid_h *= zoom_in;
    cell_w *= zoom_in;
    cell_h *= zoom_in;
    texture_rect = {0, 0, grid_w, grid_h};
    new_w = grid_w;
    new_h = grid_h;
    x = 0;
    y = 0;
}

// void Colony::map_ants(Function &&function)
// {
//     for (size_t i = 0; i < ants.size(); i++)
//         function(i, ants[i]);
// }

void View::init_hud()
{
    hud.init_hud(window, render);

    hud.create_menu("Pause");
    hud.add_rect_draw("Pause", "", {0, 0, 1, 1}, {0xFF, 0xFF, 0xFF, 0x50}, "ressources/Marianne-Regular.otf", 42);

    //hud.add_rect_draw("Pause", "premier test", {0,0,0.10,0.10}, {0xFF,0xFF,0xFF,0xFF}, "ressources/Marianne-Regular.otf", 42);
    hud.add_button("Pause", "Exit", {0.90, 0.80, 0.10, 0.10}, {0, 0xFF, 0, 0xFF}, "ressources/Marianne-Regular.otf", 21,
                   [&]() {
                       end = true;
                   });
    hud.add_button("Pause", "Restart", {0.90, 0.90, 0.10, 0.10}, {0, 0xFF, 0, 0xFF}, "ressources/Marianne-Regular.otf", 21,
                   [&]() {
                       restart = true;
                   });

    hud.hide_menu("Pause");
}

Event View::event_manager()
{
    bool close_requested = false;
    SDL_Event event;
    clicked = false;
    double_clicked = false;
    scroll = 0;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return Event::close_request;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            {
                if (hud.menu_is_hidden("Pause"))
                {
                    hud.show_menu("Pause");
                }
                else
                {
                    hud.hide_menu("Pause");
                }
                break;
            }
            default:
                break;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            mouse_x = event.button.x;
            mouse_y = event.button.y;
            if (event.button.clicks == 1)
            {
                clicked = true;
            }
            else
            {
                double_clicked = true;
            }
            break;
        case SDL_MOUSEWHEEL:
            mouse_x = event.button.x;
            mouse_y = event.button.y;
            if (event.wheel.y > 0)
            {
                scroll = 1;
            }
            else if (event.wheel.y < 0)
            {
                scroll = -1;
            }
            break;
        default:
            break;
        }
    }
    if (end)
        return Event::close_request;
    if (restart)
    {
        restart = false;
        return Event::restart;
    }
    return Event::none;
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

void View::init_grid(const Grid &grid)
{
    background_texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texture_rect.w, texture_rect.h);
    SDL_SetTextureBlendMode(background_texture, SDL_BLENDMODE_BLEND);
    entities_texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texture_rect.w, texture_rect.h);
    SDL_SetTextureBlendMode(entities_texture, SDL_BLENDMODE_BLEND);
    pheromons_texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texture_rect.w, texture_rect.h);
    SDL_SetTextureBlendMode(pheromons_texture, SDL_BLENDMODE_ADD);

    double base_tint = create_base_tint();

    for (size_t i = 0; i < grid.colonies.size(); i++)
    {
        m[grid.colonies[i]] = get_tint(i, grid.colonies.size(), base_tint, 0.35, 0.93);
        disp_pheromons[grid.colonies[i]] = false;
    }

    SDL_SetRenderTarget(render, background_texture);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 0x2C, 0x3A, 0x47, 0xFF);
    //SDL_RenderSetScale(render, 2, 2);
    for (int i = 0; i < SPACE_HEIGHT; i++)
    {
        for (int j = 0; j < SPACE_WIDTH; j++)
        {
            SDL_Rect rect = {j * cell_w, i * cell_h, cell_w, cell_h};
            SDL_RenderDrawRect(render, &rect);
        }
    }
    //SDL_RenderSetScale(render, 1, 1);
    SDL_SetRenderTarget(render, NULL);
    init_entities(grid);
    update_pheromons(grid, 0);
}

void View::init_entities(const Grid &grid)
{
    SDL_SetRenderTarget(render, entities_texture);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    for (Cell *cell : grid.map)
    {
        if (cell->is_nest())
        {
            rgb color = m[cell->get_nest()];
            draw_cell_rect(cell->get_location(), color.r, color.g, color.b, color.a);
        }
        if (cell->has_ant())
        {
            Ant *a = cell->get_ant();
            rgb color = m[a->get_colony()];
            draw_cell_circle(cell->get_location(), color.r, color.g, color.b, color.a);
        }
        if (cell->has_sugar())
        {
            draw_cell_rect(cell->get_location(), 0xFF, 0xFF, 0xFF, 0xFF);

            //(c, 0xFF, 0xFF, 0xFF);
        }
    }
    SDL_SetRenderTarget(render, NULL);
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

void View::clear()
{
    m.clear();
    disp_pheromons.clear();
    delta.clear();
    SDL_DestroyTexture(background_texture);
    SDL_DestroyTexture(entities_texture);
    SDL_DestroyTexture(pheromons_texture);
}