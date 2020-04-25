#pragma once

#include <vector>

#include "types.hpp"
#include "path.hpp"

using namespace std;

class terrain
{
private:
    int rows;
    int columns;
    vector<vector<field>> layout;
    int agent_x;
    int agent_y;
    field get_xy(int x, int y);

public:
    terrain();
    terrain(int rows, int columns);
    terrain &add_row(vector<int> row);
    path_next eval(vector<agent_move> path_seq);
};