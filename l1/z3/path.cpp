#include <vector>
#include <random>
#include <algorithm>

#include "types.hpp"
#include "path.hpp"

using namespace std;

path &path::append(agent_move next)
{
    move_sequence.push_back(next);
    return *this;
}

path path::tweak()
{
    default_random_engine gen;
    uniform_int_distribution<int> dis(0, move_sequence.size() - 1);

    vector<agent_move> copy = move_sequence;

    int k = dis(gen);
    int j = dis(gen);
    int i = dis(gen);

    if (k > j)
        swap(k, j);
    if (i < 0.4 * (move_sequence.size() - 1))
        swap(copy[k], copy[j]);
    else
        reverse(copy.begin() + k, copy.begin() + j);
    return path(copy);
}

vector<agent_move> path::get_seq()
{
    return move_sequence;
}