#pragma once

#include "types.hpp"
#include "path.hpp"
#include "terrain.hpp"

using namespace std;

class solver
{
private:
    terrain map;
    int max_tabu_size;
    int max_tweaks;
    bool better_path(path i, path j);
    agent_move random_move();

public:
    solver(terrain map, int max_tabu_size, int max_tweaks);
    path gen_path_naive();
    path solve(int s, path x0);
};