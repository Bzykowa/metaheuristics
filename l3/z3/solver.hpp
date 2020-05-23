#pragma once

#include "types.hpp"
#include "path.hpp"
#include "terrain.hpp"

using namespace std;

class solver
{
private:
    terrain map;
    int max_population;
    vector<path> initial_solutions;
    int ideal_solution_indicator;
    int get_cost(path i);
    tuple<path, path> crossover(path parent1, path parent2);
    int natural_selection(vector<int> fitnesses);

public:
    solver(terrain map, int max_population, vector<path> initial_solutions);
    path solve(int s);
};