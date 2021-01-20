import numpy as np
from scipy import stats
import matplotlib.pyplot as pltV
from sklearn.neighbors import KernelDensity

fig, plt = pltV.subplots(1,1)

lamb=1.5 
t = stats.expon.rvs(size=20,scale=1/lamb)
c = stats.expon.rvs(size=80,scale=1/lamb)
j = stats.expon.rvs(size=150,scale=1/lamb)

print(t)
print(c)
print(j)
print(stats.expon.fit(t)) 
print(stats.expon.fit(c)) 
print(stats.expon.fit(j)) 

x242 = np.linspace(0, 8).reshape(-1, 1)
expo1 = stats.expon.pdf(x242)
plt.plot(expo1, 'r-')

kde1 = KernelDensity(kernel='exponential').fit(x242)
norm2412 = np.exp(kde1.score_samples(x242))

plt.plot(norm2412)
plt.plot(stats.expon.cdf(expo1),'g-')
pltV.show()