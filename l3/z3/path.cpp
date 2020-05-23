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

path path::tweak()
{
    random_device dev;
    mt19937 gen(dev());
    uniform_int_distribution<int> dis(0, move_sequence.size() - 1);
    int i = dis(gen), j = dis(gen);
    swap(move_sequence[i], move_sequence[j]);
    return clean_seq(move_sequence);
}

vector<agent_move> path::get_seq()
{
    return move_sequence;
}

void path::trim(int index)
{
    vector<agent_move> new_seq;
    for (int i = 0; i < index; i++)
    {
        new_seq.push_back(move_sequence[i]);
    }
    move_sequence = new_seq;
}