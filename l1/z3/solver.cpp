#include <vector>
#include <chrono>
#include <random>
#include <iostream>

#include "types.hpp"
#include "path.hpp"
#include "terrain.hpp"
#include "solver.hpp"

using namespace std;

solver::solver(terrain map, int max_tabu_size, int max_tweaks)
{
    this->map = map;
    this->max_tabu_size = max_tabu_size;
    this->max_tweaks = max_tweaks;
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
    auto i_eval = map.eval(i);
    auto j_eval = map.eval(j);

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

    uniform_int_distribution<int> dis(0, 1);
    default_random_engine gen;

    auto dir = random_move();
    auto eval = map.eval(p);

    while (eval.successful() == ERR)
    {
        eval = map.eval(p);
        if (eval.fields[dir] == WALL)
        {
            dir = dis(gen) ? agent_move((dir + 1) % 4) : agent_move((dir - 1) % 4);
            continue;
        }

        p.append(dir);
    }

    return p.append(eval.successful());
}

path solver::solve(int s, path x0)
{
    //check if element is in tabu
    auto in = [](vector<vector<agent_move>> paths, vector<agent_move> path) {
        if (count(paths.begin(), paths.end(), path))
            return true;
        else
            return false;
    };

    vector<vector<agent_move>> tabu; //vector of path sequences
    path current = x0;
    path best = path(x0.get_seq());

    tabu.push_back(current.get_seq());

    //start timer
    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    chrono::duration<double> elapsed_s = end - start;

    while (elapsed_s.count() < s)
    {
        if (tabu.size() > max_tabu_size)
            tabu.erase(tabu.begin());

        path tweaked = current.tweak();

        for (int i = 0; i < max_tweaks; i++)
        {
            //update timer
            end = chrono::steady_clock::now();
            elapsed_s = end - start;
            if (elapsed_s.count() > s)
                break;

            path better_tweak = current.tweak();

            if (!in(tabu, better_tweak.get_seq()) && (better_path(better_tweak, tweaked) || in(tabu, tweaked.get_seq())))
                tweaked = better_tweak;
        }

        if (!in(tabu, tweaked.get_seq()))
        {
            current = tweaked;
            tabu.push_back(tweaked.get_seq());
        }

        if (better_path(current, best))
            best = current;
    }

    vector<agent_move> seq;
    vector<agent_move> best_seq = best.get_seq();
    path clean_best = path(seq);
    int index = 0;

    auto cancelling_move = [](vector<agent_move> seq, int i, agent_move j, agent_move k) {
        return (seq[i] == j && seq[i + 1] == k) || (seq[i] == k && seq[i + 1] == j);
    };

    while (index < best_seq.size() - 1)
    {
        if (cancelling_move(best_seq, index, L, R) || cancelling_move(best_seq, index, U, D))
            index += 2;
        else
        {
            clean_best.append(best_seq[index]);
            index++;
        }
    }
    clean_best.append(best_seq[best_seq.size() - 1]);
    return clean_best;
}