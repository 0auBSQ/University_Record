import numpy as np
from scipy import stats
import matplotlib.pyplot as pltV
from sklearn.neighbors import KernelDensity

fig, plt = pltV.subplots(1,1)

r = stats.norm.rvs(size=20)
z = stats.norm.rvs(size=80)
v = stats.norm.rvs(size=150)

print(r)
print(z)
print(v)
print(stats.norm.fit(r))
print(stats.norm.fit(z))
print(stats.norm.fit(v))

x241 = np.linspace(-5, 5).reshape(-1,1)
norm241 = stats.norm.pdf(x241)
plt.plot(norm241, 'r-')

kde = KernelDensity(kernel='gaussian').fit(x241)
norm2412 = np.exp(kde.score_samples(x241))

plt.plot(norm2412)
pltV.show()