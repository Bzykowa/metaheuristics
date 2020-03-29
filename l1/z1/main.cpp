#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <vector>
#include <tuple>
#include <limits>

using namespace std;

double h(double x1, double x2, double x3, double x4)
{
    auto norm_sq = [](double x1, double x2, double x3, double x4) {
        return x1 * x1 + x2 * x2 + x3 * x3 + x4 * x4;
    };
    double sum = x1 + x2 + x3 + x4;
    return pow(pow((norm_sq(x1, x2, x3, x4) - 4), 2), 0.125) + 0.25 * (0.5 * norm_sq(x1, x2, x3, x4) + sum) + 0.5;
}

double g(double x1, double x2, double x3, double x4)
{
    double it[] = {x1, x2, x3, x4};
    double sum = 0.0;
    double product = 1.0;
    for (int i = 0; i < 4; i++)
    {
        sum += pow(it[i], 2);
        product *= cos(it[i] / sqrt(i + 1));
    }
    return 1.0 + (sum / 4000.0) - product;
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

void tweak_xs(tuple<bool, bool, bool, bool> plus, array<double, 4> vec, double step)
{
    vec[0] = get<0>(plus) ? vec[0] + step : vec[0] - step;
    vec[1] = get<1>(plus) ? vec[1] + step : vec[1] - step;
    vec[2] = get<2>(plus) ? vec[2] + step : vec[2] - step;
    vec[3] = get<3>(plus) ? vec[3] + step : vec[3] - step;
}

int main()
{
    double fmin;
    double fcurr;
    int t, b;
    vector<tuple<bool, bool, bool, bool>> neighbours = gen_neighbours();

    cin >> t >> b;

    bool cat = b ? false : true;
    double step = cat ? 0.0001 : 0.001;

    default_random_engine gen;
    uniform_real_distribution<double> dis_h(-2.0, 2.0);
    uniform_real_distribution<double> dis_g(-10.0, 10);

    array<double, 4> xs;
    array<double, 4> minxs;
    fmin = numeric_limits<double>::max();

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    chrono::duration<double> elapsed_s = end - start;

    while (elapsed_s.count() < t)
    {
        //gen x0
        for (int i = 0; i < 4; i++)
        {
            xs[i] = cat ? dis_h(gen) : dis_g(gen);
        }
        fcurr = cat ? h(xs[0], xs[1], xs[2], xs[3]) : g(xs[0], xs[1], xs[2], xs[3]);
        if (fcurr <= fmin)
        {
            fmin = fcurr;
            minxs = xs;
        }
        bool cont = false;
        do
        {
            //search within neighbours
            for (int i = 0; i < neighbours.size(); i++)
            {
                array<double, 4> tw = xs;
                tweak_xs(neighbours[i], tw, step);
                double ftw = cat ? h(tw[0], tw[1], tw[2], tw[3]) : g(tw[0], tw[1], tw[2], tw[3]);
                //new potential min found, don't restart
                if (ftw < fcurr)
                {
                    xs = tw;
                    cont = true;
                    break;
                }
            }
        } while (cont);

        //update min if necessary
        fmin = fcurr <= fmin ? fcurr : fmin;
        minxs = fcurr <= fmin ? xs : minxs;

        //timer update
        end = chrono::steady_clock::now();
        elapsed_s = end - start;
    }
    cout << minxs[0] << " " << minxs[1] << " " << minxs[2] << " " << minxs[3] << " " << fmin << endl;
}