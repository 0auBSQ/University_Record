import numpy as np
from scipy import stats
import matplotlib.pyplot as pltV
from sklearn.neighbors import KernelDensity

fig, plt = pltV.subplots(1,1)

vals = [0.499, 0.509, 0.501, 0.494, 0.498, 0.497, 0.504, 0.506, 0.502, 0.496, 0.495, 0.493, 0.507, 0.505, 0.503, 0.491]
moyenne = sum(vals, 0.0) / len(vals)
print(moyenne)
plt.hist(vals, bins = 100)
pltV.show()
mean, sigma = np.mean(vals), np.std(vals)
print(stats.norm.interval(0.95, loc=mean, scale=sigma))
print(stats.norm.interval(0.99, loc=mean, scale=sigma))

vals2 = [85.06, 91.44, 87.93, 89.02, 87.28, 82.34, 86.23, 84.16, 88.56, 90.45, 84.91, 89.90, 85.52, 86.75, 88.54, 87.90]
mean, sigma = np.mean(vals2), np.std(vals2)
print(stats.norm.interval(0.95, loc=mean, scale=sigma))
print(stats.norm.interval(0.99, loc=mean, scale=sigma))