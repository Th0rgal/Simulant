#include "view.hpp"

View::View(bool fullScreen = false) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    if (!fullScreen)
        window = SDL_CreateWindow("Ant", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_ALLOW_HIGHDPI);
    else
        window = SDL_CreateWindow("Ant", 0, 0, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALLOW_HIGHDPI);

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

View::View(int w, int h) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    window = SDL_CreateWindow("Ant", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_ALLOW_HIGHDPI);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

View::~View() {
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool    View::event_manager() {
    bool        close_requested;
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
    
}

void    View::disp_map(std::array<Cell *, SPACE_WIDTH * SPACE_HEIGHT>  map) {

}