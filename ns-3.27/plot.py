#Authors:: Abhishek Singh(abhishek.singh199418@gmail.com)  at University of Rochester 
#Rights reserved to both author and University of Rochester


import sys
import os
from pylab import *
import numpy as np

ext = "_simulation"

data = ["direct", "leach", "modified"]

Direct = []
Leach = []
Modified = []
x_axis = []
#change directory to your directory which can be found out using pwd command on terminal of linux OS
DIR = "/home/ece448/finalproject/ns-3.27"
for name in data:
	for file in os.listdir(DIR):
		if file.find(name + ext) >= 0 and file.endswith(".txt"):
			fname = DIR + '/' + file
			for line in open(fname).readlines():
				value = float(line.split()[0])
				if name == "direct":
					Direct.append(value * 100)
				if name == "leach":
					Leach.append(value * 100)
				if name == "modified":
					Modified.append(value * 100)



size = max(len(Modified), len(Leach))

#print size

#size = len(Modified) + 100000
for i in range(0,size):
	x_axis.append(i)


zero = size - len(Direct)
for i in range(0,zero):
	Direct.append(0)

if len(Leach) < len(Modified):
	zero = size - len(Leach)
	for i in range(0,zero):
		Leach.append(0)

if len(Leach) > len(Modified):
        zero = size - len(Modified)
        for i in range(0,zero):
                Modified.append(0)

plot(x_axis, Direct, '-r', label='direct')
plot(x_axis, Leach, '-b', label='leach')
plot(x_axis, Modified, '-g', label='modified')


xlabel('rounds')
ylabel('avg_energy_ntwk')	
legend()
show()


