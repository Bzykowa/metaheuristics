#pragma once

#include <vector>
#include "types.hpp"

using namespace std;

class path
{
private:
    vector<agent_move> move_sequence;

public:
    path();
    path(vector<agent_move> seq) : move_sequence{move(seq)}{};
    path &append(agent_move next);
    path tweak();
    vector<agent_move> get_seq();
};