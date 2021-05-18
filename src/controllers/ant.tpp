template <class Function>
void Colony::map_ants(Function &&function)
{
    for (Ant *ant : ants)
        function(ant);
}