reset
set logscale x 10
set xlabel 'N'
set ylabel 'time(sec)'
set style fill solid
set title 'time comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'

plot [1000:][0:] 'result_clock_gettime.csv' using 1:2 smooth csplines lw 2 title 'Baseline', \
'' using 1:3 smooth csplines lw 2 title 'OpenMP (2 threads)', \
'' using 1:4 smooth csplines lw 2 title 'OpenMP (4 threads)', \
'' using 1:5 smooth csplines lw 2 title 'AVX', \
'' using 1:6 smooth csplines lw 2 title 'AVX + unroll looping'

reset
set logscale x 2
set xlabel 'N'
set ylabel 'rate'
set style fill solid
set title 'error rate'
set term png enhanced font 'Verdana,10'
set output 'error_rate.png'

plot [1000:100000][0:] 'error_rate.txt' using 1:2 smooth csplines lw 2 title 'Baseline', \
'' using 1:3 smooth csplines lw 2 title 'OpenMP (2 threads)', \
'' using 1:4 smooth csplines lw 2 title 'OpenMP (4 threads)', \
'' using 1:5 smooth csplines lw 2 title 'AVX', \
'' using 1:6 smooth csplines lw 2 title 'AVX + unroll looping'