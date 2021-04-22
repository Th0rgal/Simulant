#include "controllers/ant.hpp"
#include "controllers/game.hpp"
#include <vector>
// # include <SDL2/SDL.h>
// # include <SDL2/SDL_timer.h>

//todo: remove
std::vector<Ant> create_ants_array(const std::vector<Coordinates> &coordinates)
{
    std::vector<Ant> output;
    output.reserve(coordinates.size());
    for (size_t i = 0; i < coordinates.size(); i++)
        output.emplace_back(coordinates[i], i);
    return output;
}

int main()
{
    // SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    // SDL_Window *window = SDL_CreateWindow("Ant", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 0); // => Creation fenetre SDL
    // SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // bool    close_requested = false;

    // SDL_Event event;
    // while (!close_requested) {
    //     while (SDL_PollEvent(&event))
    //     {
    //         switch (event.type)
    //         {
    //             case SDL_QUIT:
    //                 close_requested = true;
    //                 break;
    //             case SDL_KEYDOWN:
    //                 switch (event.key.keysym.sym)
    //                 {
    //                     case SDLK_ESCAPE:
    //                         close_requested = true;
    //                         break;
    //                 }
    //         }
    //     }
    // }

    Game game;
    game.start();
    //create_ants_array({Coordinates(1, -2)});
    return 0;
}