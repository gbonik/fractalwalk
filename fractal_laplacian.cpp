#include <limits>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "fractal_laplacian.h"

using namespace std;

void extend_eigenfunction(double* f, int len,
                          double p, double lambda)
{
    double q = 1.0 - p;
    double d = (q - lambda) * (1 + p - lambda);

    f[(len - 1)*3] = f[len - 1];
    for (int i = len - 2; i >= 0; --i)
    {
        double a = f[i], b = f[i + 1];
        f[i*3]     = a;
        f[i*3 + 1] = (q * (1 - lambda) * a + p * q * b) / d;
        f[i*3 + 2] = (q * (1 - lambda) * b + p * q * a) / d;
    }
}

// R(z)
double decimation_poly(double p, double z)
{
    double pq = (1 - p) * p;
    return z*(2 + pq + z*(-3 + z)) / pq;
}

// R'(z)
double decimation_poly_deriv(double p, double z)
{
    double pq = (1 - p) * p;
    return (2 + pq + z*(-6 + 3*z)) / pq;
}

// Find a solution to R(z) = value with Newton method,
// starting at x.
double find_root(double p, double value, double x)
{
    double old_x = 0.0;
    int iter = 0;
    do {
        old_x = x;
        double f = decimation_poly(p, old_x) - value;
        x = old_x - f / decimation_poly_deriv(p, old_x);
    } while (fabs(x - old_x) > 2*numeric_limits<double>::epsilon() && ++iter < 300);
    return x;
}

int decimate_eigenvalue(double p, double lambda, double new_lambda[3])
{
    if (lambda == 0.0)
    {
        new_lambda[0] = 0.0;
        new_lambda[2] = 2.0 - p;
        return 2;
    }
    else if (lambda == 2.0)
    {
        new_lambda[0] = p;
        new_lambda[2] = 2.0;
        return 2;
    }

    new_lambda[0] = find_root(p, lambda, 0.0);
    new_lambda[1] = find_root(p, lambda, 1.0);
    new_lambda[2] = find_root(p, lambda, 2.0);
    return 3;
}


double compute_eigenpair(double p, int level, int* path, double* func)
{
    func[0] = 1.0;
    func[1] = (path[0] == 0 ? 1.0 : -1.0);
    double lambda = (path[0] == 0 ? 0.0 : 2.0);
    int len = 2;

    for (int i = 1; i <= level; ++i)
    {
        double new_lambda[3];
        decimate_eigenvalue(p, lambda, new_lambda);
        lambda = new_lambda[path[i]];
        if (fabs(lambda - 0.5) < 1e-14)
        {
            int sign = 1;
            for (int j = 0; j < len - 1; ++j)
            {
                func[j*3] = sign;
                func[j*3 + 1] = 0.5 * sign;
                func[j*3 + 2] = -0.5 * sign;
                sign = -sign;
            }
            func[(len - 1)*3] = sign;
        }
        else if (fabs(lambda - 1.5) < 1e-14)
        {
            for (int j = 0; j < len - 1; ++j)
            {
                func[j*3] = 1.0;
                func[j*3 + 1] = -0.5;
                func[j*3 + 2] = -0.5;
            }
            func[(len - 1)*3] = 1.0;
        }
        else
            extend_eigenfunction(func, len, p, lambda);
        len = (len - 1)*3 + 1;
    }

    return lambda;
}


int size_for_level(int level)
{
    int r = 1;
    for (int i = 0; i < level; ++i)
        r *= 3;
    return r + 1;
}


static void do_compute_laplacian(const double* f, int len, double p, double* lap)
{
    if (len < 4) return;

    int i = (len - 1) / 3;
    lap[i] = f[i] - (1 - p) * f[i - 1] - p * f[i + 1];
    int j = 2 * i;
    lap[j] = f[j] - (1 - p) * f[j + 1] - p * f[j - 1];

    do_compute_laplacian(f, i + 1, p, lap);
    do_compute_laplacian(f + i, i + 1, p, lap + i);
    do_compute_laplacian(f + j, i + 1, p, lap + j);
}


void compute_laplacian(const double* f, int len, double p, double* lap)
{
    lap[0] = f[0] - f[1];
    lap[len - 1] = f[len - 1] - f[len - 2];

    do_compute_laplacian(f, len, p, lap);
}

vector<int> eigenvalue_number_to_path(int eig)
{
    int level = 0;
    int pow_of_3 = 1;
    while (pow_of_3 + 1 <= eig)
    {
        pow_of_3 *= 3;
        ++level;
    }

    vector<int> path;

    while (level >= 0)
    {
        int prev_n = pow_of_3 / 3 + 1;
        if (eig < prev_n)
            path.push_back(0);
        else if (eig < prev_n*2 - 2)
        {
            path.push_back(1);
            int i = eig - prev_n;
            eig = prev_n - 2 - i;
        }
        else
        {
            path.push_back(2);
            eig -= (prev_n*2 - 2);
        }

        pow_of_3 /= 3;
        --level;
    }

    reverse(path.begin(), path.end());
    return path;
}


int path_to_eigenvalue_number(const int* path, int len)
{
    if (len < 1)
        return 0;

    int eig = path[0] ? 1 : 0;
    int pow_of_3 = 1;
    for (int i = 1; i < len; ++i)
    {
        if (path[i] == 1)
            eig = pow_of_3 * 2 - eig;
        else if (path[i] == 2)
            eig = pow_of_3 * 2 + eig;

        pow_of_3 *= 3;
    }

    return eig;
}



