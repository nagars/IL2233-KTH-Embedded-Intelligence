'''
For spectral analysis, we will apply an important
technique to characterize rhythmic behavior in data - the Fourier transform and power
spectral density or “spectrum” - and many subtleties associated with this technique.
1. For the EEG data set, draw the line plot, histogram, density plot, box plot, lag-1
plot, ACF and PACF graphs (lags up to 50).
2. Show the statistical characteristics of the EEG data, such as mean, variance, standard deviation.
3. Compute the auto-covariance of the EEG data. Draw and save the auto-covariance
graph. Examine the auto-covariance plot, Why does the auto-covariance exhibit
repeated peaks and troughs approximately every 0.0166 s?
4. Compute and plot the power-spectrum of the EEG data. Show it in both linear scale
and log (dB) scale. To emphasize low-amplitude rhythms hidden by large-amplitude
oscillations is to change the scale of the spectrum to decibels
'''

from scipy.io import loadmat # Import function to read data.
import matplotlib.pyplot as plt
from pandas import read_csv
from pandas import  Series
import numpy
from statsmodels.graphics.tsaplots import plot_acf, plot_pacf
from statsmodels.tsa.stattools import acovf

def plot(series, lags):
    plt.figure()
    series.plot.line(title = 'EEG - Line plot')
    plt.figure()
    series.plot.hist(title = 'EEG - Histogram')
    # density plot
    plt.figure()
    series.plot.density(title = "EEG - Density plot")
    # box plot
    plt.figure()
    series.plot.box(title = 'EEG - Box plot')
    # lag-1 plot
    plt.figure()
    import pandas as pd
    pd.plotting.lag_plot(series, lag=1)
    # acf graph
    plot_acf(series, lags=lags, title="EEG - ACF plot")
    plt.show()
    plot_pacf(series, lags=lags, title="EEG - PACF plot")
    plt.show()

def plot_autocovariance(wave, lag=50):

    auto_cov = Series(acovf(wave, nlag = lag))
    auto_cov.index = wave.index[0:101]
    auto_cov.plot.line(title = "EEG - Auto-covariance plot")
    plt.show()

def plot_power_spectrum_linear(wave, sampling_rate, title = "EEG - Power Spectrum Linear Scale"):
    
    fft_result = numpy.fft.rfft(wave)
    freqs = numpy.fft.rfftfreq(len(wave), 1/sampling_rate)
    psd = numpy.abs(fft_result)**2

    plt.xlim(0, 100)
    plt.plot(freqs, psd)
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Power [$\mu V^2$/Hz]')
    plt.title(title)
    plt.grid(True)
    plt.show()


def plot_power_spectrum_db(wave, sampling_rate, title = "EEG - Power Spectrum Log Scale"):
    
    fft_result = numpy.fft.rfft(wave)
    freqs = numpy.fft.rfftfreq(len(wave), 1/sampling_rate)
    psd = numpy.abs(fft_result)**2

    plt.plot(freqs, 10 * numpy.log10(psd))
    #plt.xlim(0, 100)
    plt.ylim(-30,61)
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Power [dB]')
    plt.title(title)
    plt.grid(True)
    plt.show()


data = loadmat('03_EEG-1.mat')
data.keys()
EEGraw = data['EEG'].reshape(-1)
t = data['t'][0]
sampling_rate = 1000

EEG = Series(EEGraw)

# Plot histogram, density plot, acf, pacf etc
plot(EEG, 50)

# Auto-Covariance
plot_autocovariance(EEG, 100)

# Show data characteristics
print('Mean: ', EEG.mean())
print('Variance: ', EEG.var())
print('Standard deviation: ', EEG.std())

# Power Spectrum
plot_power_spectrum_linear(EEG, sampling_rate)
plot_power_spectrum_db(EEG, sampling_rate)





