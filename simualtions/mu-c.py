from numpy import genfromtxt
import matplotlib.pyplot as plt
import numpy as np

data = genfromtxt("mu-c.csv", delimiter=',') 

fig, ax1 = plt.subplots()
color = 'tab:red'
ax1.set_xlabel('c')
ax1.set_ylabel('mu', color=color)
ax1.plot(data[:,0], data[:,1], label='linear scale', color=color)
ax1.tick_params(axis='y', labelcolor=color)
#ax1.legend()

ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
color = 'tab:blue'
ax2.set_ylabel('mu', color=color)  # we already handled the x-label with ax1
ax2.plot(data[:,0], data[:,1], label='logarithmic scale', color=color)
ax2.set_yscale('log')
ax2.tick_params(axis='y', labelcolor=color)
#ax2.legend()
fig.legend(loc='upper right', bbox_to_anchor=(0.9, 0.88))
plt.axvline(0.7, color='tab:purple', linestyle=':')
ticks = [0.7]
for i in data[:,0]:
	if (10*i)%5 == 0:
		ticks.append(i)
plt.xticks(ticks)
#fig.tight_layout()  # otherwise the right y-label is slightly clipped
plt.show()
'''
log = np.log(data[:, 1])
plt.subplot(211)
plt.plot(data[:, 0], data[:, 1], label = 'origin', color = 'b')
plt.ylabel('mu')
plt.legend()
plt.subplot(212)
plt.plot(data[:, 0], log, label = 'log', color = 'r')
plt.ylabel('mu')
plt.xlabel('c')
plt.legend()


index = (log > 0)
c = data[:, 0][index]
loglog = np.log(log[index])
plt.subplot(313)
plt.plot(c, loglog, label = 'loglog', color = 'y')
plt.ylabel('loglog(mu)')
plt.xlabel('c')
plt.legend();
'''

plt.show()
