#include <vector>
#include <algorithm>

#include "types.hpp"
#include "path.hpp"
#include "terrain.hpp"

using namespace std;

terrain::terrain(){
    agent_x = -1;
    agent_y = -1;
}

terrain::terrain(int rows, int columns)
{
    this->rows = rows;
    this->columns = columns;
    agent_x = -1;
    agent_y = -1;
}

terrain &terrain::add_row(vector<int> row)
{
    vector<field> row_fields;
    for (auto v : row)
    {
        field curr = field(v);
        if (curr == AGENT)
        {
            agent_x = row_fields.size();
            agent_y = layout.size();
        }
        row_fields.push_back(curr);
    }
    layout.push_back(row_fields);
    return *this;
}

field terrain::get_xy(int x, int y)
{
    return (x < columns && x > -1 && y < rows && y > -1) ? layout[y][x] : ERROR;
}

path_next terrain::eval(vector<agent_move> path_seq)
{
    int curr_row = agent_y;
    int curr_col = agent_x;
    int index = 0;
    for (auto move : path_seq)
    {
        switch (move)
        {
        case U:
        {
            curr_row--;
            break;
        }
        case D:
        {
            curr_row++;
            break;
        }
        case L:
        {
            curr_col--;
            break;
        }
        case R:
        {
            curr_col++;
            break;
        }
        }

        index++;
        field next = get_xy(curr_col, curr_row);

        if (next == WALL || next == ERROR)
        {
            return path_next(BLOCKED, index + 1);
        }
        else if (next == EXIT)
        {
            return path_next(SUCCESS, index + 1);
        }
    }

    map<agent_move, field> possible_moves;
    possible_moves.insert(pair<agent_move, field>(U, get_xy(curr_col, curr_row - 1)));
    possible_moves.insert(pair<agent_move, field>(D, get_xy(curr_col, curr_row + 1)));
    possible_moves.insert(pair<agent_move, field>(L, get_xy(curr_col - 1, curr_row)));
    possible_moves.insert(pair<agent_move, field>(R, get_xy(curr_col + 1, curr_row)));

    return path_next(possible_moves, WALKING, path_seq.size());
}