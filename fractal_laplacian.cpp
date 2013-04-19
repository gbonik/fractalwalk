#include <limits>
#include <cmath>

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
    } while (fabs(x - old_x) > 2*numeric_limits<double>::epsilon() && ++iter < 200);
    return x;
}

int decimate_eigenvalue(double p, double lambda, double new_lambda[3])
{
    if (lambda == 0.0)
    {
        new_lambda[0] = 0.0;
        new_lambda[1] = find_root(p, lambda, p < 0.5 ? 2.0 : 1.0);
        return 2;
    }
    else if (lambda == 2.0)
    {
        new_lambda[0] = find_root(p, lambda, p < 0.5 ? 0.0 : 1.0);
        new_lambda[1] = 2.0;
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

