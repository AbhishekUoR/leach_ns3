set terminal post eps color enhanced
set output "power-rrpaa.eps"
set title "Average transmit power (AP to STA) vs time"
set xlabel "Time (seconds)"
set ylabel "Power (mW)"
plot "-"  title "Average Transmit Power" with lines
5 7.55916
15 6.63028
25 29.4225
35 31.4641
45 33.8468
e
