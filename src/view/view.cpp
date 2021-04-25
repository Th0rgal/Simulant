#include "view/view.hpp"
#include <algorithm>

View::View(bool fullScreen)
{
    int w;
    int h;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    if (!fullScreen)
        window = SDL_CreateWindow("Ant", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_ALLOW_HIGHDPI);
    else
        window = SDL_CreateWindow("Ant", 0, 0, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALLOW_HIGHDPI);

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);

    SDL_GL_GetDrawableSize(window, &window_w, &window_h);
    SDL_GetWindowSize(window, &w, &h);

    scale_high_dpi = window_w / (double)w;

    init_grid();
}

View::View(int w, int h)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    window = SDL_CreateWindow("Ant", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_ALLOW_HIGHDPI);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
    SDL_GL_GetDrawableSize(window, &window_w, &window_h);

    scale_high_dpi = window_w / (double)w;
    
    init_grid();
}

View::~View()
{
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    m.clear();
    disp_pheromons.clear();
    SDL_DestroyTexture(grid_texture);
    SDL_DestroyTexture(entities_texture);
    SDL_DestroyTexture(pheromons_texture);

    SDL_Quit();
}

void View::init_grid()
{
    int cell_size = std::min(window_w / SPACE_WIDTH, window_h / SPACE_HEIGHT);

    cell_w = cell_size;
    cell_h = cell_size;
    grid_x = 0;
    grid_y = 0;
    right = {0, 0, 0, 0};
    left = {0, 0, 0, 0};
    top = {0, 0, 0, 0};
    down = {0, 0, 0, 0};

    grid_w = cell_w * SPACE_WIDTH;
    grid_h = cell_h * SPACE_HEIGHT;
    if (grid_w < window_w)
    {
        int diff = window_w - grid_w;
        grid_x = diff / 2;

        right.x = window_w - diff / 2;
        right.y = 0;
        right.w = diff / 2;
        right.h = window_h;

        left.x = 0;
        left.y = 0;
        left.w = diff / 2;
        left.h = window_h;
    }
    if (grid_h < window_h)
    {
        int diff = window_h - grid_h;
        grid_y = diff / 2;

        top.x = 0;
        top.y = 0;
        top.w = window_w;
        top.h = diff / 2;

        down.x = 0;
        down.y = window_h - diff / 2;
        down.w = window_w;
        down.h = diff / 2;
    }
}


Event View::event_manager()
{
    bool close_requested = false;
    SDL_Event event;
    clicked = false;

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
                return Event::close_request;
            case SDLK_SPACE:
                return Event::restart;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            clicked = true;
            mouse_x = event.button.x;
            mouse_y = event.button.y;
        }
    }
    return Event::none;
}

void View::draw_cell_rect(const Coordinates &c, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    int x = (c.x - X_MIN) * cell_w + grid_x;
    int y = (c.y - Y_MIN) * cell_h + grid_y;
    int w = cell_w;
    int h = cell_h;
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(render, r, g, b, a);
    SDL_RenderFillRect(render, &rect);
}

void View::draw_cell_circle(const Coordinates &c, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    int x = (c.x - X_MIN) * cell_w + cell_w / 2 + grid_x;
    int y = (c.y - Y_MIN) * cell_h + cell_h / 2 + grid_y;
    int rayon = cell_w / 4;

    //void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
    //std::cout << a << std::endl;
    SDL_SetRenderDrawColor(render, r, g, b, a);
    SDL_RenderFillCircle(render, x, y, rayon);
    //SDL_RenderFillRect(render, &rect);
}

void    View::init_grid(const Grid &grid) {
    grid_texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window_w, window_h);
    SDL_SetTextureBlendMode(grid_texture, SDL_BLENDMODE_BLEND);
    entities_texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window_w, window_h);
    SDL_SetTextureBlendMode(entities_texture, SDL_BLENDMODE_BLEND);
    pheromons_texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window_w, window_h);
    SDL_SetTextureBlendMode(pheromons_texture, SDL_BLENDMODE_ADD);

    double base_tint = create_base_tint();

    for (size_t i = 0; i < grid.colonies.size(); i++)
    {
        m[grid.colonies[i]] = get_tint(i, grid.colonies.size(), base_tint, 0.35, 0.93);
        disp_pheromons[grid.colonies[i]] = false;
    }

    SDL_SetRenderTarget(render, grid_texture);

    SDL_SetRenderDrawColor(render, 15, 17, 34, 0xFF);
    SDL_RenderClear(render);

    SDL_SetRenderDrawColor(render, 0x2C, 0x3A, 0x47, 0xFF);
    for (int i = 0; i < SPACE_HEIGHT; i++)
    {
        for (int j = 0; j < SPACE_WIDTH; j++)
        {
            SDL_Rect rect = {j * cell_w + grid_x, i * cell_h + grid_y, cell_w, cell_h};
            SDL_RenderDrawRect(render, &rect);
        }
    }
    SDL_SetRenderTarget(render, NULL);
    init_entities(grid);
    update_pheromons(grid);
}

void    View::init_entities(const Grid &grid) {
    SDL_SetRenderTarget(render, entities_texture);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    for (Cell *cell : grid.map)
    {
        Coordinates c = cell->get_location();
        if (cell->is_nest())
        {
            rgb color = m[cell->get_nest()];
            draw_cell_rect(c, color.r * 255, color.g * 255, color.b * 255, 255);
        }
        if (cell->has_ant())
        {
            Ant *a = cell->get_ant();
            rgb color = m[a->colony];
            draw_cell_circle(c, color.r * 255, color.g * 255, color.b * 255, 255);
        }
        if (cell->has_sugar()) {
            draw_cell_rect(c, 0xFF, 0xFF, 0xFF, 0xFF);

           //(c, 0xFF, 0xFF, 0xFF);
        }
    }
    SDL_SetRenderTarget(render, NULL);
}

void    View::update_pheromons(const Grid &grid) {
    SDL_SetRenderTarget(render, pheromons_texture);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    for (Cell *cell : grid.map) {
        Coordinates c = cell->get_location();
        for (Colony *colony : grid.colonies)
        {
            if (disp_pheromons[colony]) {
                double alpha = cell->get_nest_pheromons(colony);
                rgb color = m[colony];
                draw_cell_rect(c, color.r * 255, color.g * 255, color.b * 255, std::max(0.0, alpha * 255 - 100));
            }
        }
    }
    SDL_SetRenderTarget(render, NULL);
}

void    View::update_entities(const Grid &grid) {
    SDL_SetRenderTarget(render, pheromons_texture);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    SDL_SetTextureBlendMode(pheromons_texture, SDL_BLENDMODE_NONE);
    for (Action action : delta) {
        if (action.type == ActionType::ant_move) {
            Cell *from = action.state_begin[0];
            Cell *to = action.state_begin[1];

            draw_cell_rect(from->get_location(), 0, 0, 0, 0);
            draw_cell_rect(to->get_location(), 0, 0, 0, 0);
            //do the animation
        }
    }
    SDL_SetTextureBlendMode(pheromons_texture, SDL_BLENDMODE_BLEND);
}


void    View::clear() {
    std::cout << "je detruit une map de taille : " << m.size() << std::endl;
    
    m.clear();
    disp_pheromons.clear();
    SDL_DestroyTexture(grid_texture);
    SDL_DestroyTexture(entities_texture);
    SDL_DestroyTexture(pheromons_texture);


}
