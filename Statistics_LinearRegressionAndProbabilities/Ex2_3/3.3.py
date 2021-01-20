import numpy as np
from scipy import stats
import matplotlib.pyplot as pltV
from sklearn.neighbors import KernelDensity

fig, plt = pltV.subplots(1,1)

n = 1
p = 1/2
rand = stats.binom.rvs(n, p, size=500)
print(rand)
mean, sigma = np.mean(rand), np.std(rand)

print(stats.norm.interval(0.99, loc=mean, scale=sigma))