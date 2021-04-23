template <class Function>
void Grid::map_colonie(Function &&function)
{
    for (Colonie &colonie : colonies) {
        colonie.map_ants(function);
    }
}