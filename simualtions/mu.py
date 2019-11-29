import numpy as np
import matplotlib.pyplot as plt
from numpy import genfromtxt

num = 11
repeat = 10
fig, ax = plt.subplots(nrows=2, ncols=1, sharex=True)

data = genfromtxt("mu-rtime.csv", delimiter=',') 
x = []
mean = []
std = []

for i in range(num):
	x.append(data[i*repeat, 0])
	runs = data[i*repeat:(i+1)*repeat, 1]
	mean.append(np.mean(runs, axis=0))
	std.append(np.std(runs, axis=0)) 

ax[0].errorbar(x, mean, yerr=std, marker='o', linestyle='None', capsize=2, color='tab:red')
ax[0].set_ylabel('time (thousand steps)')

data = genfromtxt("mu-level.csv", delimiter=',') 
x = []
mean = []
std = []

for i in range(num):
	x.append(data[i*repeat, 0])
	runs = data[i*repeat:(i+1)*repeat, 1]
	mean.append(np.mean(runs, axis=0))
	std.append(np.std(runs, axis=0)) 

ax[1].errorbar(x, mean, yerr=std, marker='o', linestyle='None', capsize=2, color='tab:blue')
ax[1].set_ylabel('number of visited levels')
ax[1].set_xlabel('mu')
plt.show()