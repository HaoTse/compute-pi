#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "computepi.h"

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0

#define compute_error(pi) ( (pi > M_PI) ? pi - M_PI : M_PI - pi)

int main(int argc, char const *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};
    double time_record[SAMPLE_SIZE];
    double min, max;
    FILE *fp_time = fopen("result_clock_gettime.csv", "a");
    FILE *fp_error = fopen("error_rate.txt", "a");

    if (argc < 2) return -1;

    int N = atoi(argv[1]);
    //SAMPLE_SIZE define in "compute.h"
    int i, loop = SAMPLE_SIZE;

    // Baseline
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_baseline(N);
        clock_gettime(CLOCK_ID, &end);
        time_record[i] = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    fprintf(fp_error, "%d %lf ", N, compute_error(compute_pi_baseline(N))/M_PI);
    fprintf(fp_time, "%d, %lf, ", N, compute_ci(&min, &max, time_record));
    //printf("compute_pi_baseline(%d) 95%%ci is [%lf, %lf]\n", N, min, max);


    // OpenMP with 2 threads
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_openmp(N, 2);
        clock_gettime(CLOCK_ID, &end);
        time_record[i] = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    fprintf(fp_error, "%lf ", compute_error(compute_pi_openmp(N, 2))/M_PI);
    fprintf(fp_time, "%lf, ", compute_ci(&min, &max, time_record));
    //printf("compute_pi_openmp_2(%d) 95%%ci is [%lf, %lf]\n", N, min, max);


    // OpenMP with 4 threads
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_openmp(N, 4);
        clock_gettime(CLOCK_ID, &end);
        time_record[i] = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    fprintf(fp_error, "%lf ", compute_error(compute_pi_openmp(N, 4))/M_PI);
    fprintf(fp_time, "%lf, ", compute_ci(&min, &max, time_record));
    //printf("compute_pi_openmp_4(%d) 95%%ci is [%lf, %lf]\n", N, min, max);


    // AVX SIMD
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_avx(N);
        clock_gettime(CLOCK_ID, &end);
        time_record[i] = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    fprintf(fp_error, "%lf ", compute_error(compute_pi_avx(N))/M_PI);
    fprintf(fp_time, "%lf, ", compute_ci(&min, &max, time_record));
    //printf("compute_pi_avx(%d) 95%%ci is [%lf, %lf]\n", N, min, max);


    // AVX SIMD + Loop unrolling
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_avx_unroll(N);
        clock_gettime(CLOCK_ID, &end);
        time_record[i] = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    fprintf(fp_error, "%lf\n", compute_error(compute_pi_avx_unroll(N))/M_PI);
    fprintf(fp_time, "%lf\n", compute_ci(&min, &max, time_record));
    //printf("compute_pi_avx_unroll(%d) 95%%ci is [%lf, %lf]\n", N, min, max);
    fclose(fp_time);
    fclose(fp_error);

    return 0;
}
