#include "controllers/game.hpp"
#include "controllers/random.hpp"
#include "models/action.hpp"
#include <iostream>
#include <chrono>

Game::Game() : grid(3), view(600, 600) // 3 colonie
{
}

void Game::restart()
{
    grid.clear();
    grid = Grid(3);

    view.clear();

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
        {
            restart();
        }
        long delay = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - previousTime).count();
        if (delay < minimal_delay)
        {
            view.update(delay / (double)minimal_delay, grid);
            continue;
        }
        loop(delay);
        previousTime = std::chrono::high_resolution_clock::now();
    }
}

void Game::loop(unsigned long delay)
{
    std::cout << delay << std::endl;
    grid.map_ants([&](size_t i, Ant *ant) {
        std::vector<Coordinates> possible_moves = ant->find_moves(grid);
        if (!possible_moves.empty())
        {
            Cell *next_cell = grid.get_cell(possible_moves[random_index(0, possible_moves.size() - 1)]); // todo: use pheromons
            if (next_cell->has_ant())
            {
                Ant *enemy = next_cell->get_ant();
                Colony *colony = enemy->get_colony();
                colony->remove_ant(grid, colony->find_ant_index(enemy));
                ant->move(next_cell);
            }
            else
            {
            }
        }
    });
}