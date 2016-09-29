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
    double time_record;
    double min, max;
    FILE *fp_time = fopen("result_clock_gettime.csv", "a");
    FILE *fp_error = fopen("error_rate.txt", "a");
    FILE *fp_ci = fopen("ci.txt", "r");

    if (argc < 2) return -1;

    int N = atoi(argv[1]);
    //SAMPLE_SIZE define in "compute.h"
    int i, loop = SAMPLE_SIZE;
    double mean = 0.0;

    // Baseline
    mean = 0.0;
    fscanf(fp_ci, "%lf %lf", &min, &max);
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_baseline(N);
        clock_gettime(CLOCK_ID, &end);
        time_record = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
        if(time_record < min && time_record > max)
            i--;
        else
            mean += time_record;
    }
    fprintf(fp_error, "%d %lf ", N, compute_error(compute_pi_baseline(N))/M_PI);
    mean /= (double)SAMPLE_SIZE;
    fprintf(fp_time, "%d, %lf, ", N, mean);


    // OpenMP with 2 threads
    mean = 0.0;
    fscanf(fp_ci, "%lf %lf", &min, &max);
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_openmp(N, 2);
        clock_gettime(CLOCK_ID, &end);
        time_record = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
        if(time_record < min && time_record > max)
            i--;
        else
            mean += time_record;
    }
    fprintf(fp_error, "%lf ", compute_error(compute_pi_openmp(N, 2))/M_PI);
    mean /= (double)SAMPLE_SIZE;
    fprintf(fp_time, "%lf, ", mean);


    // OpenMP with 4 threads
    mean = 0.0;
    fscanf(fp_ci, "%lf %lf", &min, &max);
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_openmp(N, 4);
        clock_gettime(CLOCK_ID, &end);
        time_record = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
        if(time_record < min && time_record > max)
            i--;
        else
            mean += time_record;
    }
    fprintf(fp_error, "%lf ", compute_error(compute_pi_openmp(N, 4))/M_PI);
    mean /= (double)SAMPLE_SIZE;
    fprintf(fp_time, "%lf, ", mean);


    // AVX SIMD
    mean = 0.0;
    fscanf(fp_ci, "%lf %lf", &min, &max);
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_avx(N);
        clock_gettime(CLOCK_ID, &end);
        time_record = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
        if(time_record < min && time_record > max)
            i--;
        else
            mean += time_record;
    }
    fprintf(fp_error, "%lf ", compute_error(compute_pi_avx(N))/M_PI);
    mean /= (double)SAMPLE_SIZE;
    fprintf(fp_time, "%lf, ", mean);


    // AVX SIMD + Loop unrolling
    mean = 0.0;
    fscanf(fp_ci, "%lf %lf", &min, &max);
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_avx_unroll(N);
        clock_gettime(CLOCK_ID, &end);
        time_record = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
        if(time_record < min && time_record > max)
            i--;
        else
            mean += time_record;
    }
    fprintf(fp_error, "%lf ", compute_error(compute_pi_avx_unroll(N))/M_PI);
    mean /= (double)SAMPLE_SIZE;
    fprintf(fp_time, "%lf, ", mean);


    // LEIBNIZ
    mean = 0.0;
    fscanf(fp_ci, "%lf %lf", &min, &max);
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_leibniz(N);
        clock_gettime(CLOCK_ID, &end);
        time_record = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
        if(time_record < min && time_record > max)
            i--;
        else
            mean += time_record;
    }
    fprintf(fp_error, "%lf\n", compute_error(compute_pi_leibniz(N))/M_PI);
    mean /= (double)SAMPLE_SIZE;
    fprintf(fp_time, "%lf\n", mean);
    
    fclose(fp_time);
    fclose(fp_error);
    fclose(fp_ci);

    return 0;
}
