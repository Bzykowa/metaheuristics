#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <limits>

using namespace std;

int main()
{
    int t;                         //time limit
    int n;                         //number of cities
    vector<vector<int>> distances; //distances between cities
    vector<int> min_path;          //best solution
    int min_cost;                  //best path length
    const int tn = 1000;           //max tabu size
    const int perm_it = 10000;     //max permutations in main loop
    vector<vector<int>> tabu;
    vector<int> curr_path;
    int curr_cost;

    cin >> t >> n;

    //read distances
    for (int i = 0; i < n; i++)
    {
        vector<int> new_row;
        for (int j = 0; j < n; j++)
        {
            int new_val;
            cin >> new_val;
            new_row.push_back(new_val);
        }
        distances.push_back(new_row);
    }

    /*
    //generate starting path random
    auto gen_path_rand = [](vector<int> path, int n) {
        path.clear();
        for (int i = 1; i <= n; i++)
        {
            path.push_back(i);
        }
        random_shuffle(path.begin(), path.end());
        path.push_back(path[0]);
        return path;
    };
    */

    //generate starting path naively
    auto gen_path_naive = [](vector<vector<int>> dist, int n) {
        vector<int> path;
        int current = 0;
        while (path.size() != n)
        {
            int min_dist = numeric_limits<int>::max();
            int min_i = numeric_limits<int>::max();

            for (int i = 0; i < n; i++)
            {
                if (dist[current][i] < min_dist && !count(path.begin(), path.end(), i))
                {
                    min_dist = dist[current][i];
                    min_i = i;
                }
            }

            path.push_back(min_i);
            current = min_i;
        }
        path.push_back(path[0]);
        return path;
    };

    //path length
    auto path_len = [](vector<vector<int>> dist, vector<int> path, int n) {
        int sum = 0;
        for (int i = 0; i < n; i++)
            sum += dist[path[i]][path[i + 1]];
        return sum;
    };

    //random modification (swap values or reverse a slice)
    auto tweak = [](vector<int> path, int n) {
        default_random_engine gen;
        uniform_int_distribution<int> dis(1, n - 1);
        vector<int> copy = path;
        int k = dis(gen);
        int j = dis(gen);
        int i = dis(gen);
        if (k > j)
            swap(k, j);
        if (i < 0.9 * (n - 1))
            swap(copy[k], copy[j]);
        else
            reverse(copy.begin() + k, copy.begin() + j);
        return copy;
    };

    //check if element is in tabu
    auto in = [](vector<vector<int>> paths, vector<int> path) {
        if (count(paths.begin(), paths.end(), path))
            return true;
        else
            return false;
    };

    //rng from 1 to n
    default_random_engine gen;
    uniform_int_distribution<int> dis(1, n);

    //generate x0
    curr_path = gen_path_naive(distances, n);
    curr_cost = path_len(distances, curr_path, n);
    min_path = curr_path;
    min_cost = curr_cost;
    tabu.push_back(curr_path);

    //start timer
    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    chrono::duration<double> elapsed_s = end - start;

    while (elapsed_s.count() < t)
    {
        if (tabu.size() > tn)
            tabu.erase(tabu.begin());
        vector<int> tweaked_path = tweak(curr_path, n);
        int tweaked_cost = path_len(distances, tweaked_path, n);

        for (int i = 0; i < perm_it; i++)
        {
            //update timer
            end = chrono::steady_clock::now();
            elapsed_s = end - start;
            if (elapsed_s.count() > t)
                break;

            //set new perm
            vector<int> tweaked_again_path = tweak(curr_path, n);
            int tweaked_again_cost = path_len(distances, tweaked_again_path, n);

            if (!in(tabu, tweaked_again_path) && (tweaked_again_cost < tweaked_cost || in(tabu, tweaked_path)))
            {
                tweaked_path = tweaked_again_path;
                tweaked_cost = tweaked_again_cost;
            }
        }
        if (!in(tabu, tweaked_path))
        {
            curr_path = tweaked_path;
            curr_cost = tweaked_cost;
            tabu.push_back(curr_path);
        }

        if (curr_cost < min_cost)
        {
            min_path = curr_path;
            min_cost = curr_cost;
        }
    }

    //print result
    cout << min_cost << endl;

    for (auto i : min_path)
    {
        cout << i << " ";
    }
    cout << endl;
}