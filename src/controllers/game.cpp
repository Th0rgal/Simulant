#include "controllers/game.hpp"
#include <iostream>
#include <chrono>

Game::Game() : grid(3), view(700, 700) // 3 colonie
{
}

void Game::start()
{
    unsigned long minimal_delay = 1e9; // one second
    std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
    view.disp_grid(grid);
    while (!view.event_manager())
    {
        long delay = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - previousTime).count();
        if (delay < minimal_delay)
            continue;
        loop(delay);
        previousTime = std::chrono::high_resolution_clock::now();
    }
}

void Game::loop(unsigned long delay)
{
    grid.map_ants([&](Ant &ant) {
        ant.find_move();
    });
    //std::cout << delay << std::endl;
}