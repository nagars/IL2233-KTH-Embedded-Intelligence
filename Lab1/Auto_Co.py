from statsmodels.tsa.stattools import acovf
from statsmodels.tsa.stattools import acf
from statsmodels.graphics.tsaplots import plot_acf
from pandas import Series
import numpy
import matplotlib.pyplot as plt

array = [1, 2, 3, 4, 5, 6, 7, 8, 7, 6, 5, 4, 3, 2, 1]

# Autocovariance
auto_cov_series = Series(acovf(array))
print ("Autcovariance")
print(numpy.array(auto_cov_series))

#Autocorrelation
print("Autcorrelation")
print(acf(Series(array)))
plot_acf(Series(array))
plt.ylim(-1.2,1.2)
plt.show()

