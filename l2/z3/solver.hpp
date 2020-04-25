#pragma once

#include "types.hpp"
#include "path.hpp"
#include "terrain.hpp"

using namespace std;

class solver
{
private:
    terrain map;
    long int temp;
    double c;
    int ideal_solution_indicator;
    bool better_path(path i, path j);
    agent_move random_move();
    bool is_ideal();

public:
    solver(terrain map, long int temp, double c);
    path gen_path_naive();
    path solve(int s, path x0);
};