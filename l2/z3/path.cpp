#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>

#include "types.hpp"
#include "path.hpp"
#include "terrain.hpp"

using namespace std;

path &path::append(agent_move next)
{
    move_sequence.push_back(next);
    return *this;
}

bool path::cancelling_move(vector<agent_move> seq, int i, agent_move j, agent_move k)
{
    return (seq[i] == j && seq[i + 1] == k) || (seq[i] == k && seq[i + 1] == j);
}

path path::clean_seq(vector<agent_move> seq)
{
    path clean_best = path(seq);
    int index = 0;

    while (index < (int)seq.size() - 1)
    {
        if (cancelling_move(seq, index, L, R) || cancelling_move(seq, index, U, D))
            index += 2;
        else
        {
            clean_best.append(seq[index]);
            index++;
        }
    }
    clean_best.append(seq[seq.size() - 1]);
    return clean_best;
}

path path::tweak(double rem_time, terrain map)
{
    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    chrono::duration<double> elapsed_s = end - start;
    random_device dev;
    mt19937 gen(dev());

    vector<agent_move> copy = move_sequence;

    while (elapsed_s.count() < rem_time)
    {
        vector<int> dir_swaps;

        for (int i = 0; i < (int)copy.size() - 1; i++)
        {
            if (copy[i] != copy[i + 1])
            {
                dir_swaps.push_back(i);
            }
        }

        if (dir_swaps.size() > 0)
        {
            uniform_int_distribution<int> dis(0, dir_swaps.size() - 1);
            int index = dis(gen);
            swap(copy[index], copy[index + 1]);
            path_next state = map.eval(copy);

            if (state.state == SUCCESS)
            {
                int i = 0;
                vector<agent_move> ret;
                while (i < state.length)
                {
                    ret.push_back(copy[i]);
                    i++;
                }
                return clean_seq(ret);
            }
            else
            {
                swap(copy[index], copy[index + 1]);
            }

            end = chrono::steady_clock::now();
            elapsed_s = end - start;
        }
        else
        {
            break;
        }
    }
    return clean_seq(copy);
}

vector<agent_move> path::get_seq()
{
    return move_sequence;
}