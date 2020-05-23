#include <iostream>
#include <vector>
#include <string>

#include "types.hpp"
#include "path.hpp"
#include "terrain.hpp"
#include "solver.hpp"

using namespace std;

int main()
{
    int t, n, m, s, p; //t - time limit, n - rows, m - columns, s - initial solutions, p - max population
    vector<string> lines;
    vector<string> sol_strings;
    vector<path> solutions;

    //read input
    cin >> t >> n >> m >> s >> p;

    for (int i = 0; i < n; i++)
    {
        string t;
        cin >> t;
        lines.push_back(t);
    }
    for (int i = 0; i < s; i++)
    {
        string t;
        cin >> t;
        sol_strings.push_back(t);
    }

    terrain map = terrain(n, m);

    //parse input to something usable
    for (auto v : lines)
    {
        vector<int> row;
        for (int i = 0; i < m; i++)
        {
            int field = v[i] - '0';
            row.push_back(field);
        }
        map.add_row(row);
    }

    auto parse_path = [](string seq) {
        vector<agent_move> path_seq;
        for (int i = 0; i < seq.size(); i++)
        {
            if (seq[i] == 'U')
                path_seq.push_back(U);
            else if (seq[i] == 'L')
                path_seq.push_back(L);
            else if (seq[i] == 'D')
                path_seq.push_back(D);
            else if (seq[i] == 'R')
                path_seq.push_back(R);
        }

        return path(path_seq);
    };

    for (auto i : sol_strings)
        solutions.push_back(parse_path(i));

    auto printer = [](agent_move x) {
        switch (x)
        {
        case U:
        {
            return "U";
            break;
        }
        case R:
        {
            return "R";
            break;
        }
        case D:
        {
            return "D";
            break;
        }
        case L:
        {
            return "L";
            break;
        }
        }
        return "ERR";
    };

    solver ga = solver(map, 100000, solutions);
    path best = ga.solve(t);

    vector<agent_move> best_seq = best.get_seq();

    cout << best_seq.size() << endl;

    for (auto v : best_seq)
    {
        cout << printer(v);
    }

    cout << endl;
}