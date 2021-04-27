#include "controllers/game.hpp"
#include "controllers/random.hpp"
#include "models/action.hpp"
#include <iostream>
#include <chrono>

//std::vector<Action> delta;

Game::Game() : grid(4), view(false) // 3 colonie
{
}

void Game::restart()
{
    grid.clear();
    grid = Grid(3);

    delta.clear();

    view.clear();

    view.init_grid(grid);
    view.renderAll();
}

void Game::start()
{
    unsigned long minimal_delay = 1e9; // one second
    std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
    view.init_grid(grid);
    view.renderAll();
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
            view.renderAll();
            continue;
        }
        loop(delay);
        view.update_map(delta);
        previousTime = std::chrono::high_resolution_clock::now();
    }
}

Action move_ant_on_screen(Grid &grid, Ant *ant, Cell *cell)
{
    Action action;
    action.type = ActionType::AntMove;
    action.updated.push_back(ant->get_location());
    action.updated.push_back(cell->get_location());
    action.colony = ant->get_colony();
    return action;
}

void Game::loop(unsigned long delay)
{
    delta.clear();
    std::vector<Ant *> killed;
    std::vector<Ant *> in_fight;
    grid.map_ants([&](size_t i, Ant *ant) {
        if (std::find(killed.begin(), killed.end(), ant) == killed.end() && std::find(in_fight.begin(), in_fight.end(), ant) == in_fight.end())
        {
            std::vector<Coordinates> possible_moves = ant->find_moves(grid);
            if (!possible_moves.empty())
            {
                Cell *next_cell = grid.get_cell(possible_moves[random_index(0, possible_moves.size() - 1)]); // todo: use pheromons

                // fight
                if (next_cell->has_ant())
                {
                    // me move the attacker to the new case on the screen
                    delta.push_back(move_ant_on_screen(grid, ant, next_cell));

                    if (flip_a_coin())
                    {
                        killed.push_back(next_cell->get_ant());
                        ant->move(grid, next_cell);
                    }
                    else
                    {
                        killed.push_back(ant);
                        in_fight.push_back(next_cell->get_ant());
                    }
                }

                // let's take sugar
                else if (!ant->has_sugar() && next_cell->has_sugar())
                    ant->add_sugar();

                // let's deposit sugar
                else if (ant->has_sugar() && next_cell->is_nest())
                    ant->deposit_sugar();

                // juste move
                else
                {
                    delta.push_back(move_ant_on_screen(grid, ant, next_cell));
                    ant->move(grid, next_cell);
                }
            }
        }
    });

    grid.map_colony([&](Colony *colony) {
        for (size_t i = colony->spawn_ants() + 1; i > 0; i--)
        {
            // todo: spawn ant
        }
    });

    for (Ant *ant : killed)
    {
        Action action;
        action.type = ActionType::AntDeath;
        action.updated.push_back(ant->get_location());
        // winner's colony
        action.colony = grid.get_cell(ant->get_location())->get_ant()->get_colony();
        delta.push_back(action);
        Colony *colony = ant->get_colony();
        colony->remove_ant(grid, colony->find_ant_index(ant));
    }
}
