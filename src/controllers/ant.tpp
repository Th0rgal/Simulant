template <class Function>
void Colony::map_ants(Function &&function)
{
    for (size_t i = 0; i < ants.size(); i++)
        function(i, ants[i]);
}