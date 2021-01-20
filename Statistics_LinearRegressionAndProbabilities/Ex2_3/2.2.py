import numpy as np
from scipy import stats
import matplotlib.pyplot as pltV
from sklearn.neighbors import KernelDensity

fig, plt = pltV.subplots(1,1)

x = np.linspace(-10, 10)
normal = stats.norm.pdf(x, 0, 1)
normal1 = stats.norm.pdf(x, 2, 1.5)
normal2 = stats.norm.pdf(x, 2, 0.6)

plt.plot(normal)
plt.plot(normal1)
plt.plot(normal2)
pltV.show()