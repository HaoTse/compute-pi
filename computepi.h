#include <stdio.h>

#define SAMPLE_SIZE 25

double compute_pi_baseline(size_t N);
double compute_pi_openmp(size_t N, int threads);
double compute_pi_avx(size_t N);
double compute_pi_avx_unroll(size_t N);
double compute_pi_leibniz(size_t N);

/*Calculate 95% confidence interval
  store the interval [min, max] in the first two parameters
  with a set of data which has SAMPLE_SIZE elements
  return mean value*/
double compute_ci(double *min, double *max, double data[SAMPLE_SIZE]);
