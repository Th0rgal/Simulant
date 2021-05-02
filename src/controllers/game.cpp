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

Action move_ant_on_screen(Grid &grid, Ant *ant, Coordinates location)
{
    Action action;
    action.type = ActionType::AntMove;
    action.updated.push_back(ant->get_location());
    action.updated.push_back(location);
    action.colony = ant->get_colony();
    return action;
}

Coordinates chose(std::map<Coordinates, double> possible_moves)
{
    double stop = random_double(0, 1);
    for (auto iterator = possible_moves.begin(); iterator != possible_moves.end(); iterator++)
        if ((stop -= iterator->second) < 0)
            return iterator->first;

    return std::next(possible_moves.begin(), random_index(0, possible_moves.size() - 1))->first;
}

void Game::loop(unsigned long delay)
{
    delta.clear();
    std::vector<Ant *> killed;
    std::vector<Ant *> in_fight;
    grid.map_ants([&](size_t i, Ant *ant) {
        if (std::find(killed.begin(), killed.end(), ant) == killed.end() && std::find(in_fight.begin(), in_fight.end(), ant) == in_fight.end())
        {
            std::map<Coordinates, double> possible_moves = ant->find_moves(grid);
            if (!possible_moves.empty())
            {
                Cell *next_cell = grid.get_cell(chose(possible_moves)); // todo: use pheromons

                // fight
                if (next_cell->has_ant())
                {
                    // me move the attacker to the new case on the screen
                    delta.push_back(move_ant_on_screen(grid, ant, next_cell->get_location()));

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
                {
                    ant->add_sugar();
                    delta.push_back(move_ant_on_screen(grid, ant, ant->get_location()));
                }

                // let's deposit sugar
                else if (next_cell->is_nest() && next_cell->get_nest() == ant->get_colony())
                {
                    if (ant->has_sugar())
                        ant->deposit_sugar();
                    delta.push_back(move_ant_on_screen(grid, ant, ant->get_location()));
                }

                // juste move
                else
                {
                    delta.push_back(move_ant_on_screen(grid, ant, next_cell->get_location()));
                    ant->move(grid, next_cell);
                }
            }
            else
                delta.push_back(move_ant_on_screen(grid, ant, ant->get_location()));
        }
        else
            delta.push_back(move_ant_on_screen(grid, ant, ant->get_location()));
    });

    grid.map_colony([&](Colony *colony) { grid.spawn_ants(colony, colony->spawn_ants()); });

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
