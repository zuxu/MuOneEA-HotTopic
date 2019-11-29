import numpy as np
import matplotlib.pyplot as plt
from numpy import genfromtxt

repeat = 10
mu = [30, 50, 70]
color = ['r', 'g', 'b']

fig, ax = plt.subplots(nrows=2, sharex=True)
#ax[0].set_title('Density of 0-bits v.s. time')
#ax[1].set_title('Remaining levels v.s. time')

for j in range(3):
	data = []
	size = []
	for i in range(repeat):
		data.append(genfromtxt('e10000-100-0.25-0.05-0.05-1.00-%d-%d.csv' % (mu[j], i), delimiter=','))
		size.append(data[i].shape[0])

	maxt = np.max(size)
	t = range(maxt)
	mean = np.zeros((2, maxt), dtype='float32')
	#std = np.zeros((2, maxt), dtype='float32')
	mini = np.zeros((2, maxt), dtype='float32')
	maxa = np.zeros((2, maxt), dtype='float32')

	for i in t:
		onemax = []
		level = []
		for k in range(repeat):
			if data[k].shape[0] >= i+1:
				onemax.append(data[k][i, 1])
				level.append(data[k][i, 2])
			else:
				onemax.append(0.0)
				level.append(0.0)
		mean[1,i] = np.mean(level)
		mean[0,i] = np.mean(onemax)
		mini[1,i] = np.min(level)
		mini[0,i] = np.min(onemax)
		maxa[1,i] = np.max(level)
		maxa[0,i] = np.max(onemax)

	ax[1].plot(t, mean[0,:], color=color[j], label='mu=%d' % mu[j])
	ax[0].plot(t, mean[1,:], color=color[j], label='mu=%d' % mu[j])
	ax[1].fill_between(t, mini[0,:], maxa[0,:], color=color[j], alpha=0.6)
	ax[0].fill_between(t, mini[1,:], maxa[1,:], color=color[j], alpha=0.6)

ax[0].set_ylabel('density of 0-bits')
ax[0].legend()
ax[1].set_ylabel('remaining levels (percentage)')
ax[1].set_xlabel('time (thousand steps)')
#ax[1].legend()
plt.show()