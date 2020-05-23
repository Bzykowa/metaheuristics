#include <vector>
#include <chrono>
#include <random>
#include <iostream>
#include <limits>
#include <cmath>

#include "types.hpp"
#include "path.hpp"
#include "terrain.hpp"
#include "solver.hpp"

using namespace std;

solver::solver(terrain map, int max_population, vector<path> initial_solutions)
{
    this->map = map;
    this->max_population = max_population;
    this->initial_solutions = initial_solutions;
    ideal_solution_indicator = 50;
}

int solver::get_cost(path i)
{
    auto i_eval = map.eval(i.get_seq());

    if (i_eval.state == SUCCESS)
    {
        return i_eval.length;
    }
    else
        return numeric_limits<int>::max();
}

tuple<path, path> solver::crossover(path parent1, path parent2)
{
    vector<agent_move> p1 = parent1.get_seq();
    vector<agent_move> p2 = parent1.get_seq();

    int min_l = min(p1.size(), p2.size());

    random_device dev;
    mt19937 gen(dev());
    uniform_int_distribution<int> dis_a(0, min_l);
    uniform_int_distribution<int> dis_b(0, min_l);

    int i = dis_a(gen), j = dis_b(gen);

    if (i > j)
        swap(i, j);

    if (i != j)
        for (i; i < j; i++)
        {
            agent_move tmp = p1[i];
            p1[i] = p2[i];
            p2[i] = tmp;
        }

    return make_tuple(path(p1), path(p2));
}

int solver::natural_selection(vector<int> fitnesses)
{
    int p_size = fitnesses.size();

    random_device dev;
    mt19937 gen(dev());
    uniform_int_distribution<int> dis(0, p_size - 1);

    int best_ind = dis(gen);

    for (int i = 0; i < 4; i++)
    {
        int n = dis(gen);
        if (fitnesses[n] < fitnesses[best_ind])
            best_ind = n;
    }

    return best_ind;
}

path solver::solve(int s)
{
    vector<path> population = initial_solutions;

    double best_fit = 100000;
    path best = population[0];

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    auto last = chrono::steady_clock::now();
    chrono::duration<double> elapsed_s = end - start;
    chrono::duration<double> since_last;

    while (elapsed_s.count() < s)
    {
        vector<int> fitnesses;

        for (auto i : population)
        {
            int fitness = get_cost(i);
            if (fitness < numeric_limits<int>::max())
                i.trim(fitness - 1);
            fitnesses.push_back(fitness);

            if (fitness < best_fit)
            {
                best = i;
                best_fit = fitness;
                last = chrono::steady_clock::now();
            }
        }

        vector<path> pop_v2;

        for (int i = 0; i < max_population / 2; i++)
        {
            path parent_1 = population[natural_selection(fitnesses)];
            path parent_2 = population[natural_selection(fitnesses)];

            tuple<path, path> children = crossover(parent_1, parent_2);
            pop_v2.push_back((get<0>(children)).tweak());
            pop_v2.push_back((get<1>(children)).tweak());
        }

        population = pop_v2;
        end = chrono::steady_clock::now();
        since_last = end - last;
        elapsed_s = end - start;

        if (since_last.count() > log(s))
        {
            break;
        }
    }

    return best;
}