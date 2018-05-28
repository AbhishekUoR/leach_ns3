set terminal post eps color enhanced
set output "throughput-parf.eps"
set title "Throughput (AP to STA) vs time"
set xlabel "Time (seconds)"
set ylabel "Throughput (Mb/s)"
plot "-"  title "Throughput Mbits/s" with lines
5 28.7635
15 22.561
25 21.7771
35 20.4366
45 15.5632
e
