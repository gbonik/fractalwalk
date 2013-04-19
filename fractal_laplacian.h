#ifndef FRACTAL_LAPLACIAN_H
#define FRACTAL_LAPLACIAN_H

void extend_eigenfunction(double* f, int len, double p, double lambda);
double decimation_poly(double p, double z);
double decimation_poly_deriv(double p, double z);
double find_root(double p, double value, double x);
int decimate_eigenvalue(double p, double lambda, double new_lambda[3]);
double compute_eigenpair(double p, int level, int* path, double* func);
int size_for_level(int level);


#endif

