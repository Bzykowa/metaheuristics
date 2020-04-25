#pragma once

#include <map>
#include <functional>

using namespace std;

enum agent_move
{
    U,
    R,
    D,
    L,
    ERR
};

enum field
{
    ERROR = -1,
    FREE = 0,
    WALL = 1,
    AGENT = 5,
    EXIT = 8
};

enum path_state
{
    SUCCESS,
    BLOCKED,
    WALKING
};

struct path_next
{
    map<agent_move, field> fields;
    path_state state;
    int length;
    agent_move successful()
    {
        for (auto it = fields.begin(); it != fields.end(); ++it)
            if (it->second == EXIT)
                return it->first;
        return ERR;
    }

    path_next(map<agent_move, field> fields, path_state state, int length)
    {
        this->fields = fields;
        this->state = state;
        this->length = length;
    }

    path_next(path_state state, int length)
    {
        this->fields = map<agent_move, field>();
        this->state = state;
        this->length = length;
    }
};
