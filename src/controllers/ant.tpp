template <class Function>
void Colonie::map_ants(Function &&function)
{
    for (Ant &ant : ants)
        function(ant);
}