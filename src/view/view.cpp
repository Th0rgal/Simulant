#include "view.hpp"

void    View::init_grid() {
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
    if (grid_w < window_w) {
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
    if (grid_h < window_h) {
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

View::View(bool fullScreen) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    if (!fullScreen)
        window = SDL_CreateWindow("Ant", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_ALLOW_HIGHDPI);
    else
        window = SDL_CreateWindow("Ant", 0, 0, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALLOW_HIGHDPI);

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_GL_GetDrawableSize(window, &window_w, &window_h);
    init_grid();
}

View::View(int w, int h) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    window = SDL_CreateWindow("Ant", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_ALLOW_HIGHDPI);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_GL_GetDrawableSize(window, &window_w, &window_h);
    init_grid();
}

View::~View() {
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool    View::event_manager() {
    bool        close_requested = false;
    SDL_Event   event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                close_requested = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        close_requested = true;
                        break;
                }
        }
    }
    return close_requested;
}

void    View::show_grid() {
    //2C3A47
    SDL_SetRenderDrawColor(render, 0x2C, 0x3A, 0x47, 0xFF);
    SDL_RenderFillRect(render, &right);
    SDL_RenderFillRect(render, &left);
    SDL_RenderFillRect(render, &top);
    SDL_RenderFillRect(render, &down);

    //SDL_SetRenderDrawColor(render, 0xFF, 0, 0, 0xFF);
    for (int i = 0; i < SPACE_HEIGHT; i++) {
        for (int j = 0; j < SPACE_WIDTH; j++) {
            SDL_Rect rect = {j * cell_w + grid_x , i * cell_h + grid_y, cell_w, cell_h};
            SDL_RenderDrawRect(render, &rect);
        }
    }
}

void    View::draw_cell(Coordinates& c, uint8_t r, uint8_t g, uint8_t b) {
    int x = (c.x - X_MIN) * cell_w;
    int y = (c.y - Y_MIN) * cell_h;
    int w = cell_w;
    int h = cell_h;
    SDL_Rect    rect = {x, y, w, h};
    
    SDL_SetRenderDrawColor(render, r, g, b, 255);
    SDL_RenderFillRect(render, &rect);
}

void    View::disp_grid(const Grid &grid) {
    SDL_SetRenderDrawColor(render, 0x0F, 0x11, 0x22, 0xFF);
    SDL_RenderClear(render);

    show_grid();

    for (Cell* cell : grid.map) {
        Coordinates c = cell->get_location();
        //if (cell->has_sugar()) {
        //    draw_cell(c, 0xFF, 0xFF, 0xFF);
        //}
        if (cell->is_nest()) {
            draw_cell(c, 0x9A, 0xEC, 0xDB);;
        }
    }

    SDL_RenderPresent(render);

}