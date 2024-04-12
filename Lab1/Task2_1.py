'''
Data: Generate a series of five sequential sine wave signals for five seconds, each sine
wave lasting 1 second. The nth sine wave signal xn = sin(2π · n · f), where f = 10, and
n = 1, 2, 3, 4, 5, i.e., frequency 10Hz, 20Hz, 30Hz, 40Hz and 50Hz. The series is digitalized
with a sampling rate is 200 Hz.
Task:
1. Draw a line plot of the series.
2. Draw power spectrum (power density graph) of the series.
3. Draw the spectrogram of the series.
4. Draw and compare the ACF and PACF graphs of the first one-second (frequency
10Hz) and the second one-second series (frequency 20Hz), with lags up to 50.
'''

import numpy
import matplotlib.pyplot as plt
import statsmodels.api as sm
from statsmodels.graphics.tsaplots import plot_acf
from statsmodels.graphics.tsaplots import plot_pacf
from pandas import Series

def generate_sine_wave(freq, amplitude, duration, sampling_rate, noise_level):

    num_samples = round(duration * sampling_rate) + 1
    time = numpy.linspace(0, duration, num_samples)
    noise = numpy.random.uniform(-noise_level, noise_level, num_samples)
    sine_wave = amplitude * numpy.sin(2 * numpy.pi * freq * time) + noise
    return sine_wave

def plot_wave(wave, title = "Sine Wave", colour = None):
    wave.plot.line(title = title, figsize = (100,10))
    plt.xlabel('Time(s)')
    plt.ylabel('Amplitude')
    plt.grid(True)
    plt.show()


def plot_power_spectrum_density(wave, sampling_rate = 200):

    pwr_spec = generate_power_spectrum(wave, sampling_rate)  #10 Hz
    plt.plot(pwr_spec[1], pwr_spec[0]/len(pwr_spec[0]))
    
    plt.title("Power Spectrum Density")
    plt.xlabel('Frequency [Hz]')
    plt.ylabel('Power [$\mu V^2$/Hz]')
    plt.grid(True)
    plt.xlim(0, sampling_rate / 2) 
    plt.show()

def generate_power_spectrum(wave, sampling_rate = 200):
    # Perform FFT to calculate power spectrum (Keep only real values)
    fft_result = numpy.fft.rfft(wave - wave.mean(0))
    freq = numpy.fft.rfftfreq(len(wave), 1 / sampling_rate)
    power_spectrum = numpy.abs(fft_result) ** 2

    return [power_spectrum, freq]

def plot_spectrogram(wave, sampling_rate = 200):

    plt.specgram(wave, Fs=sampling_rate, NFFT=64, noverlap=32)
    plt.title('Spectrogram of Sine Wave')
    plt.xlabel('Time (s)')
    plt.ylabel('Frequency (Hz)')
    plt.colorbar(label='Power/Frequency (dB/Hz)')
    plt.show()

def plot_ACF(wave, lag = 50):

    plot_acf(wave.iloc[0:200], lags=lag, title='10Hz Autocorrelation')
    plt.show()
    plot_acf(wave.iloc[200:400], lags=lag, title='20Hz Autocorrelation')
    plt.show()

def plot_PACF(wave, lag = 50):

    plot_pacf(wave.iloc[0:200], lags=lag, title='10Hz Partial Autocorrelation')
    plt.show()
    plot_pacf(wave.iloc[200:400], lags=lag, title='20Hz Partial Autocorrelation')
    plt.show()

# Sine wave Parameters
frequency = [10, 20, 30, 40, 50]  # Hz
amplitude = 1.0
sampling_rate = 200  # Hz
duration = 1.0 - 1/sampling_rate # seconds
noise = 0

# Generate sine waves and combine
wave = []
wave.extend(generate_sine_wave(frequency[0], amplitude, duration, sampling_rate, noise))     # 10 Hz
wave.extend(generate_sine_wave(frequency[1], amplitude, duration, sampling_rate, noise))     # 20 Hz
wave.extend(generate_sine_wave(frequency[2], amplitude, duration, sampling_rate, noise))     # 30 Hz
wave.extend(generate_sine_wave(frequency[3], amplitude, duration, sampling_rate, noise))     # 40 Hz
wave.extend(generate_sine_wave(frequency[4], amplitude, duration, sampling_rate, noise))     # 50 Hz
wave = Series(wave)
wave.index = numpy.linspace(0, 5 - 1/sampling_rate, sampling_rate*5)

# Line Plots
plot_wave(wave)

# Power Spectrum
plot_power_spectrum_density(wave)

# Spectrogram
plot_spectrogram(wave)

# ACF
plot_ACF(wave)

# PACF
plot_PACF(wave)
