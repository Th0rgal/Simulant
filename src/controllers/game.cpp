#include "controllers/game.hpp"
#include "controllers/random.hpp"
#include "models/action.hpp"
#include <iostream>
#include <chrono>

//std::vector<Action> delta;

Game::Game() : grid(15), view(true) // 3 colonie
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
    unsigned long minimal_delay = 2e8; // one second
    size_t current_block = 0;
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
            view.update(delay / (double)minimal_delay, grid, current_block);
            view.renderAll();
            continue;
        }
        loop(delay, ++current_block);
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

void Game::loop(unsigned long delay, size_t current_block)
{
    delta.clear();

    grid.map_colony([&](Colony *colony) {
        std::vector<Coordinates> new_ants = spawn_list[colony];
        std::vector<Coordinates> updated_new_ants;
        for (Coordinates coordinates : new_ants)
        {
            if (grid.get_cell(coordinates)->is_void())
                grid.spawn_ant(colony, coordinates);
            else
                updated_new_ants.push_back(coordinates);
        }
        spawn_list[colony] = updated_new_ants;
    });

    std::vector<Ant *> killed;
    std::vector<Ant *> in_fight;
    grid.map_ants([&](size_t i, Ant *ant) {
        if (std::find(killed.begin(), killed.end(), ant) == killed.end() && std::find(in_fight.begin(), in_fight.end(), ant) == in_fight.end())
        {
            Cell *next_cell = ant->find_move(grid, current_block);
            if (next_cell != NULL)
            {

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
                    {
                        grid.get_cell(ant->get_location())->add_sugar_pheromon();
                        if (ant->deposit_sugar()) // if an ant spawned
                            spawn_list[ant->get_colony()].push_back(ant->get_location());
                    }
                    delta.push_back(move_ant_on_screen(grid, ant, ant->get_location()));
                }

                // juste move
                else
                {
                    if (ant->has_sugar())
                        grid.get_cell(ant->get_location())->add_sugar_pheromon();
                    delta.push_back(move_ant_on_screen(grid, ant, next_cell->get_location()));
                    ant->move(grid, next_cell);
                }
            }
            else
            {
                std::cout << "hello" << std::endl;
                delta.push_back(move_ant_on_screen(grid, ant, ant->get_location()));
            }
        }
        else
        {
            std::cout << "CETTE FOURMI EST MORT:" << ant->get_location() << std::endl;
            delta.push_back(move_ant_on_screen(grid, ant, ant->get_location()));
        }
    });

    for (Ant *ant : killed)
    {
        if (ant == NULL)
            continue;
        Action action;
        action.type = ActionType::AntDeath;
        std::cout << "killed:" << ant->get_location() << std::endl;
        action.updated.push_back(ant->get_location());
        // winner's colony
        action.colony = grid.get_cell(ant->get_location())->get_ant()->get_colony();
        delta.push_back(action);
        Colony *colony = ant->get_colony();
        colony->remove_ant(grid, colony->find_ant_index(ant));
    }
}
