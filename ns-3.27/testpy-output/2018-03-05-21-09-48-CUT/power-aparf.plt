set terminal post eps color enhanced
set output "power-aparf.eps"
set title "Average transmit power (AP to STA) vs time"
set xlabel "Time (seconds)"
set ylabel "Power (mW)"
plot "-"  title "Average Transmit Power" with lines
5 0.732568
15 6.95234
25 9.62976
35 25.8432
45 27.756
e
