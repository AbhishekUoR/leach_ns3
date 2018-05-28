set terminal post eps color enhanced
set output "throughput-aparf.eps"
set title "Throughput (AP to STA) vs time"
set xlabel "Time (seconds)"
set ylabel "Throughput (Mb/s)"
plot "-"  title "Throughput Mbits/s" with lines
5 28.7635
15 23.3562
25 18.2669
35 18.9598
45 13.8024
e
