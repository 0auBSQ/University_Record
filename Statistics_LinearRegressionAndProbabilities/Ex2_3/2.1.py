import numpy as np
from scipy import stats
import matplotlib.pyplot as pltV
from sklearn.neighbors import KernelDensity

fig, plt = pltV.subplots(1,1)

k = np.arange(0,100)
binomial = stats.binom.pmf(k,30,0.5)
binomial1 = stats.binom.pmf(k,30,0.7)
binomial2 = stats.binom.pmf(k,50,0.4)
plt.plot(binomial)
plt.plot(binomial1)
plt.plot(binomial2)
pltV.show()