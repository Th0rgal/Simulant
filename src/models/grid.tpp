template <class Function>
void Grid::map_colony(Function &&function)
{
    for (Colony &colony : colonies)
        function(colony);
}