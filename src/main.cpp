#include "controllers/ant.hpp"
#include "controllers/game.hpp"
#include "view/view.hpp"
#include <vector>
// # include <SDL2/SDL.h>
// # include <SDL2/SDL_timer.h>

//todo: remove
std::vector<Ant> create_ants_array(const std::vector<Coordinates> &coordinates)
{
    std::vector<Ant> output;
    output.reserve(coordinates.size());
    for (const Coordinates &coordinate : coordinates)
        output.emplace_back(coordinate);
    return output;
}

int main()
{
    Game game;
    game.start();
    //create_ants_array({Coordinates(1, -2)});
    return 0;
}