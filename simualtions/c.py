import numpy as np
import matplotlib.pyplot as plt
from numpy import genfromtxt

num = 11
repeat = 10
fig, ax = plt.subplots(nrows=2, ncols=1, sharex=True)

def draw(filename, L, position, color):
	
	data = genfromtxt(filename, delimiter=',') 
	x = []
	mean = []
	std = []

	for i in range(num):
		x.append(data[i*repeat, 0])
		runs = data[i*repeat:(i+1)*repeat, 1]
		mean.append(np.mean(runs, axis=0))
		std.append(np.std(runs, axis=0))

	ax[position].errorbar(x, mean, yerr=std, marker='o', linestyle='None', capsize=2, color=color, label='L = '+str(L))

draw('c-rtime-100.csv', 100, 0, 'red')
draw('c-level-100.csv', 100, 1, 'red')
draw('c-rtime-200.csv', 200, 0, 'blue')
draw('c-level-200.csv', 200, 1, 'blue')
ax[0].legend()
ax[0].set_ylabel('time (thousand steps)')
ax[1].set_ylabel('number of visited levels')
ax[1].set_xlabel('c')
plt.show()

