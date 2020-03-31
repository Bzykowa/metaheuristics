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
    int t, n, m; //t - time limit, n - rows, m - columns
    vector<string> lines;

    cin >> t >> n >> m;

    for (int i = 0; i < n; i++)
    {
        string t;
        cin >> t;
        lines.push_back(t);
    }

    terrain map = terrain(n, m);

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

    solver tabu = solver(map, 1000, 10000);
    path x0 = tabu.gen_path_naive();
    path best = tabu.solve(t, x0);

    vector<agent_move> best_seq = best.get_seq();

    cout << best_seq.size() << endl;

    for (auto v : best_seq)
    {
        cout << printer(v) << " ";
    }

    cout << endl;
}