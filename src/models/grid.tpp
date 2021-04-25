template <class Function>
void Grid::map_colony(Function &&function)
{
    for (Colony *colony : colonies)
        function(colony);
}

template <class Function>
void Grid::map_ants(Function &&function)
{
    for (Colony *colony : colonies)
        colony->map_ants([&](size_t i, Ant *ant) {
            function(i, ant);
        });
}