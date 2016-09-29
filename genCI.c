#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "computepi.h"

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0

int main(int argc, char const *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};
    double time_record[SAMPLE_SIZE];
    double min, max;
    FILE *fp_ci = fopen("ci.txt", "w");

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
    compute_ci(&min, &max, time_record);
    fprintf(fp_ci, "%lf %lf\n", min, max);


    // OpenMP with 2 threads
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_openmp(N, 2);
        clock_gettime(CLOCK_ID, &end);
        time_record[i] = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    compute_ci(&min, &max, time_record);
    fprintf(fp_ci, "%lf %lf\n", min, max);


    // OpenMP with 4 threads
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_openmp(N, 4);
        clock_gettime(CLOCK_ID, &end);
        time_record[i] = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    compute_ci(&min, &max, time_record);
    fprintf(fp_ci, "%lf %lf\n", min, max);


    // AVX SIMD
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_avx(N);
        clock_gettime(CLOCK_ID, &end);
        time_record[i] = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    compute_ci(&min, &max, time_record);
    fprintf(fp_ci, "%lf %lf\n", min, max);


    // AVX SIMD + Loop unrolling
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_avx_unroll(N);
        clock_gettime(CLOCK_ID, &end);
        time_record[i] = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    compute_ci(&min, &max, time_record);
    fprintf(fp_ci, "%lf %lf\n", min, max);

    // LEIBNIZ
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_leibniz(N);
        clock_gettime(CLOCK_ID, &end);
        time_record[i] = (double)(end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    compute_ci(&min, &max, time_record);
    fprintf(fp_ci, "%lf %lf\n", min, max);
    fclose(fp_ci);

    return 0;
}
