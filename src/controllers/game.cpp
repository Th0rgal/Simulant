#include "controllers/game.hpp"
#include <iostream>
#include <chrono>

Game::Game() : grid(3), view() // 3 colonie
{
}

void Game::restart() {
    grid = Grid(3);
    view.init_grid(grid);
}

void Game::start()
{
    unsigned long minimal_delay = 1e9; // one second
    std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
    view.init_grid(grid);
    Event event;

    while ((event = view.event_manager()) != Event::close_request)
    {
        if (event == Event::restart)
            restart();
        long delay = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - previousTime).count();
        if (delay < minimal_delay) {
            view.update(delay / (double)minimal_delay, grid);
            continue;
        }
        loop(delay);
        previousTime = std::chrono::high_resolution_clock::now();
    }
}

void Game::loop(unsigned long delay)
{
    grid.map_ants([&](Ant &ant) {
        ant.find_move();
    });
}