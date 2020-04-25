#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <vector>
#include <tuple>
#include <limits>

using namespace std;

double salomon(double x1, double x2, double x3, double x4)
{
    double square_sum = x1 * x1 + x2 * x2 + x3 * x3 + x4 * x4;
    return 1 - cos(2.0 * M_PI * sqrt(square_sum)) + 0.1 * sqrt(square_sum);
}

double salomon_grad(double x1, double x2, double x3, double x4)
{

    auto der_salomon = [](double val_at_i, double x1, double x2, double x3, double x4) {
        double der_sqrt = sqrt(x1 * x1 + x2 * x2 + x3 * x3 + x4 * x4);
        return (0.1 * val_at_i) / der_sqrt + (2.0 * M_PI * val_at_i * sin(2.0 * M_PI * der_sqrt)) / der_sqrt;
    };

    return sqrt(pow(abs(der_salomon(x1, x1, x2, x3, x4)), 2) + pow(abs(der_salomon(x2, x1, x2, x3, x4)), 2) + pow(abs(der_salomon(x3, x1, x2, x3, x4)), 2) + pow(abs(der_salomon(x4, x1, x2, x3, x4)), 2));
}

bool best_solution(double x1, double x2, double x3, double x4)
{
    if (salomon_grad(x1, x2, x3, x4) == 0)
        return true;
    else
        return false;
}

//generate neighbours to x'
//true => x[i]+step
//false => x[i]-step
vector<tuple<bool, bool, bool, bool>> gen_neighbours()
{
    vector<tuple<bool, bool, bool, bool>> res;
    tuple<bool, bool, bool, bool> tmp(false, false, false, false);
    res.push_back(tmp);
    get<0>(tmp) = true;
    res.push_back(tmp);
    get<1>(tmp) = true;
    res.push_back(tmp);
    get<2>(tmp) = true;
    res.push_back(tmp);
    get<3>(tmp) = true;
    res.push_back(tmp);
    get<0>(tmp) = false;
    res.push_back(tmp);
    get<1>(tmp) = false;
    res.push_back(tmp);
    get<2>(tmp) = false;
    res.push_back(tmp);
    get<0>(tmp) = true;
    res.push_back(tmp);
    get<1>(tmp) = true;
    res.push_back(tmp);
    get<0>(tmp) = false;
    res.push_back(tmp);
    get<3>(tmp) = false;
    res.push_back(tmp);
    get<2>(tmp) = true;
    res.push_back(tmp);
    get<1>(tmp) = false;
    res.push_back(tmp);
    get<0>(tmp) = true;
    res.push_back(tmp);
    get<3>(tmp) = true;
    res.push_back(tmp);
    return res;
}

array<double, 4> tweak_xs(tuple<bool, bool, bool, bool> plus, array<double, 4> vec)
{
    random_device dev;
    mt19937 gen(dev());
    uniform_real_distribution<double> dis_s(-0.5, 0.5);

    vec[0] = get<0>(plus) ? vec[0] + dis_s(gen) : vec[0] - dis_s(gen);
    vec[1] = get<1>(plus) ? vec[1] + dis_s(gen) : vec[1] - dis_s(gen);
    vec[2] = get<2>(plus) ? vec[2] + dis_s(gen) : vec[2] - dis_s(gen);
    vec[3] = get<3>(plus) ? vec[3] + dis_s(gen) : vec[3] - dis_s(gen);
    return vec;
}

int main()
{
    double temp = 100000000000;
    double c = 0.95;
    double fmin;
    double fcurr;
    double fprev;
    int t;
    double x1, x2, x3, x4;
    vector<tuple<bool, bool, bool, bool>> neighbours = gen_neighbours();
    random_device dev;
    mt19937 gen(dev());
    uniform_int_distribution<int> dis_i(0, 15);
    uniform_real_distribution<double> dis_01(0, 1);

    cin >> t >> x1 >> x2 >> x3 >> x4;

    array<double, 4> prev_xs = {x1, x2, x3, x4};
    array<double, 4> curr_xs = prev_xs;
    array<double, 4> minxs;
    fmin = numeric_limits<double>::max();

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    chrono::duration<double> elapsed_s = end - start;

    while (elapsed_s.count() < t && temp > 0 && !best_solution(x1, x2, x3, x4))
    {
        curr_xs = tweak_xs(neighbours[dis_i(gen)], curr_xs);

        fcurr = salomon(curr_xs[0], curr_xs[1], curr_xs[2], curr_xs[3]);
        fprev = salomon(prev_xs[0], prev_xs[1], prev_xs[2], prev_xs[3]);

        if (fcurr <= fprev || dis_01(gen) <= exp((fcurr - fprev) / temp))
        {
            prev_xs = curr_xs;
            fprev = fcurr;
        }

        temp *= c;

        if (fprev <= fmin)
        {
            fmin = fprev;
            minxs = prev_xs;
        }

        //timer update
        end = chrono::steady_clock::now();
        elapsed_s = end - start;
    }

    cout << minxs[0] << " " << minxs[1] << " " << minxs[2] << " " << minxs[3] << " " << fmin << endl;
}