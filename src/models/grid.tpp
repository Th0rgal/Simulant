template <class Function>
void Grid::map_ants(Function &&function)
{
    for (Ant &ant : ants)
        function(ant);
}