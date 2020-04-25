#include <vector>
#include <chrono>
#include <random>
#include <iostream>

#include "types.hpp"
#include "path.hpp"
#include "terrain.hpp"
#include "solver.hpp"

using namespace std;

solver::solver(terrain map, long int temp, double c)
{
    this->map = map;
    this->temp = temp;
    this->c = c;
    ideal_solution_indicator = 10;
}

agent_move solver::random_move()
{
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
    chrono::high_resolution_clock::duration d = chrono::high_resolution_clock::now() - start;
    unsigned seed = d.count();
    mt19937 gen(seed);
    uniform_int_distribution<int> dis(0, 3);
    return agent_move(dis(gen));
}

bool solver::better_path(path i, path j)
{
    auto i_eval = map.eval(i.get_seq());
    auto j_eval = map.eval(j.get_seq());

    if (i_eval.state == SUCCESS)
    {
        if (j_eval.state == SUCCESS)
        {
            return i_eval.length <= j_eval.length;
        }
        else
            return true;
    }
    else
        return false;
}

path solver::gen_path_naive()
{
    vector<agent_move> seq;
    path p = path(seq);

    auto dir = random_move();
    auto eval = map.eval(p.get_seq());

    while (eval.successful() == ERR)
    {
        eval = map.eval(p.get_seq());
        if (eval.fields[dir] == WALL)
        {
            dir = agent_move((dir + 1) % 4);
            continue;
        }

        p.append(dir);
    }

    return p.append(eval.successful());
}

bool solver::is_ideal()
{
    if (ideal_solution_indicator)
        return false;
    else
        return true;
}

path solver::solve(int s, path x0)
{
    path best = path(x0.get_seq());
    path prev = path(x0.get_seq());

    random_device dev;
    mt19937 gen(dev());
    uniform_real_distribution<double> dis(0, 1);

    //start timer
    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    chrono::duration<double> elapsed_s = end - start;

    while (elapsed_s.count() < s && temp > 0 && !is_ideal())
    {
        path current = prev.tweak(s - elapsed_s.count(), map);

        auto current_quality = map.eval(current.get_seq());
        auto prev_quality = map.eval(prev.get_seq());
        auto best_quality = map.eval(best.get_seq());

        if (current_quality.length < prev_quality.length || dis(gen) < exp((current_quality.length - prev_quality.length) / temp))
        {
            prev = current;
            prev_quality = current_quality;
        }

        temp *= c;

        if (prev_quality.length < best_quality.length)
        {
            best = prev;
            ideal_solution_indicator = 10;
        }
        else
        {
            ideal_solution_indicator--;
        }

        end = chrono::steady_clock::now();
        elapsed_s = end - start;
    }

    return best;
}