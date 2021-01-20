import numpy as np
from scipy import stats
import matplotlib.pyplot as pltV
from sklearn.neighbors import KernelDensity

fig, plt = pltV.subplots(1,1)

rand = stats.norm.rvs(size=20)
x = np.linspace(-6, 6)
normalrand = stats.norm.pdf(x)
plt.plot(x,normalrand, 'r-', lw=1, label='normPdf')
plt.hist(rand)
pltV.show()