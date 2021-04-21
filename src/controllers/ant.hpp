#include <stdlib.h>
#include <vector>

struct Coordinates
{
    Coordinates(int x, int y);
    int x;
    int y;
};

class Ant
{
public:
    Ant(Coordinates coordinates, size_t index);
    Coordinates get_location();
    size_t get_index();
    bool has_sugar();
    void add_sugar();
    void remove_sugar();
    void move();
};

struct Cell
{
    Cell(const Coordinates location);
    Coordinates get_location();
    bool is_void();

    unsigned short get_sugar_pheromons();
    bool has_sugar();
    bool is_sugar_track();
    void add_sugar();
    void remove_sugar();
    void add_sugar_pheromon();
    void reduce_sugar_pheromon();

    unsigned short get_nest_pheromons();
    bool is_nest();
    void set_nest();

    ssize_t get_ant_index();
    void set_ant(size_t antIndex);
    void remove_ant();
    void move_ant(Ant &ant, const Coordinates &new_location);
};

bool is_closer_from_nest(Cell base, Cell compared);

struct Grid
{
    Grid();
    std::vector<Coordinates> get_neighbors(const Coordinates &location);
    Cell load_cell(const Coordinates &location); //copy
    void add_cell(Cell cell);
    void linearize_nest_pheromons();
    void reduce_sugar_pheromons();
};