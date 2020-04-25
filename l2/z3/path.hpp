#pragma once

#include <vector>
#include "types.hpp"
#include "terrain.hpp"

using namespace std;

class path
{
private:
    vector<agent_move> move_sequence;
    bool cancelling_move(vector<agent_move> seq, int i, agent_move j, agent_move k);

public:
    path();
    path(vector<agent_move> seq) : move_sequence{move(seq)} {};
    path &append(agent_move next);
    path tweak(double rem_time, terrain map);
    vector<agent_move> get_seq();
    path clean_seq(vector<agent_move> seq);
};