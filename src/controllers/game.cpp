#include "controllers/game.hpp"
#include <iostream>
#include <chrono>

Game::Game()
{
}

void Game::start()
{
    unsigned long minimal_delay = 1e9; // one second
    std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
    while (true)
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
    grid.map_colonie([&](Ant &ant) {
        std::cout << ant.has_sugar();
    });
    std::cout << delay << std::endl;
}