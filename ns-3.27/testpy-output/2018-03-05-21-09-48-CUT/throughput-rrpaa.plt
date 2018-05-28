set terminal post eps color enhanced
set output "throughput-rrpaa.eps"
set title "Throughput (AP to STA) vs time"
set xlabel "Time (seconds)"
set ylabel "Throughput (Mb/s)"
plot "-"  title "Throughput Mbits/s" with lines
5 27.1731
15 23.9128
25 25.0942
35 20.2776
45 14.9498
e
