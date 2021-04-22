#include "controllers/game.hpp"
#include <iostream>
#include <chrono>

Game::Game()
{
}

void Game::start()
{
    unsigned long minimal_delay = 1e9;                           // one second
    std::chrono::high_resolution_clock::time_point previousTime; // initialized on 0
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
    std::cout << delay << std::endl;
}