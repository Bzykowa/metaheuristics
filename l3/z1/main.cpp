#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

const int arg_count = 5;
const char delimiter = ' ';
const double x_tweak = 0.1;
const int sel_tweak = 4;
const int population_size = 10;

double yang(vector<double> x, vector<double> ep)
{
    double sum = 0;

    for (int i = 1; i <= arg_count; i++)
    {
        sum += ep[i - 1] * pow(abs(x[i - 1]), i);
    }

    return sum;
}

tuple<vector<double>, vector<double>> crossover(tuple<vector<double>, vector<double>> parents)
{
    random_device dev;
    mt19937 gen(dev());
    uniform_int_distribution<int> dis_a(0, get<0>(parents).size());
    uniform_int_distribution<int> dis_b(0, get<1>(parents).size());

    vector<double> p1 = get<0>(parents);
    vector<double> p2 = get<1>(parents);

    int i = dis_a(gen), j = dis_b(gen);

    if (i > j)
        swap(i, j);

    if (i != j)
        for (i; i < j; i++)
        {
            double tmp = p1[i];
            p1[i] = p2[i];
            p2[i] = tmp;
        }

    return make_tuple(p1, p2);
}

vector<double> mutation(vector<double> x)
{
    random_device dev;
    mt19937 gen(dev());
    uniform_real_distribution<double> dis(-(x_tweak), x_tweak);

    vector<double> mutated = x;

    for (int i = 0; i < arg_count; i++)
    {
        mutated[i] *= (1 + dis(gen));
    }

    return mutated;
}

int natural_selection(vector<double> fitnesses)
{
    int p_size = fitnesses.size();

    random_device dev;
    mt19937 gen(dev());
    uniform_int_distribution<int> dis(0, p_size - 1);

    int best_ind = dis(gen);

    for (int i = 0; i < sel_tweak; i++)
    {
        int n = dis(gen);
        if (fitnesses[n] < fitnesses[best_ind])
            best_ind = n;
    }

    return best_ind;
}

tuple<vector<double>, double> ga(int time, vector<double> x, vector<double> ep)
{
    vector<vector<double>> population;

    for (int i = 0; i < population_size; i++)
    {
        population.push_back(mutation(x));
    }

    double best_fit = 100000;
    vector<double> best = x;

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    auto last = chrono::steady_clock::now();
    chrono::duration<double> elapsed_s = end - start;
    chrono::duration<double> since_last;

    while (elapsed_s.count() < time)
    {
        vector<double> fitnesses;

        for (auto i : population)
        {
            double fitness = yang(i, ep);
            fitnesses.push_back(fitness);

            if (fitness < best_fit)
            {
                best = i;
                best_fit = fitness;
                last = chrono::steady_clock::now();
            }
        }

        vector<vector<double>> pop_v2;

        for (int i = 0; i < population_size / 2; i++)
        {
            vector<double> parent_1 = population[natural_selection(fitnesses)];
            vector<double> parent_2 = population[natural_selection(fitnesses)];

            tuple<vector<double>, vector<double>> children = crossover(make_tuple(parent_1, parent_2));
            pop_v2.push_back(mutation(get<0>(children)));
            pop_v2.push_back(mutation(get<1>(children)));
        }

        population = pop_v2;
        end = chrono::steady_clock::now();
        since_last = end - last;
        elapsed_s = end - start;

        if (since_last.count() > log(time))
        {
            break;
        }
    }

    return make_tuple(best, best_fit);
}

int main()
{
    string input;
    int t;
    vector<double> xs;
    vector<double> eps;

    //input processing
    getline(cin, input);
    stringstream ss(input);
    string token;

    getline(ss, token, delimiter);
    t = stoi(token);

    for (int i = 1; i <= 5; i++)
    {
        getline(ss, token, delimiter);
        xs.push_back(stod(token));
    }

    for (int i = 1; i <= 5; i++)
    {
        getline(ss, token, delimiter);
        eps.push_back(stod(token));
    }

    //get results
    tuple<vector<double>, double> results = ga(t, xs, eps);

    for (auto i : get<0>(results))
        cout << i << " ";
    cout << get<1>(results) << endl;
}