set terminal post eps color enhanced
set output "power-parf.eps"
set title "Average transmit power (AP to STA) vs time"
set xlabel "Time (seconds)"
set ylabel "Power (mW)"
plot "-"  title "Average Transmit Power" with lines
5 1.09281
15 6.62224
25 28.1172
35 33.0653
45 36.458
e
