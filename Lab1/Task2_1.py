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
import array
from statsmodels.tsa.stattools import acf
from statsmodels.tsa.stattools import pacf

class sine_wave:
    def __init__(self, wave, name):
        self.name = name
        self.wave = wave

class power_spectrum:
    def __init__(self, spectrum, name):
        self.name = name
        self.spectrum = spectrum[0]
        self.freq = spectrum[1]

def generate_sine_wave(freq, amplitude, duration, sampling_rate, noise_level):

    num_samples = int(duration * sampling_rate)
    time = numpy.linspace(0, duration, num_samples)
    noise = numpy.random.uniform(-noise_level, noise_level, num_samples)
    sine_wave = amplitude * numpy.sin(2 * numpy.pi * freq * time) + noise
    return sine_wave

def plot_wave(plt, wave, title = "Sine Wave", colour = None):
    plt.plot(wave.wave, label=wave.name, color=colour)
    plt.title(title)
    plt.xlabel('Sample')
    plt.ylabel('Amplitude')
    plt.grid(True)

    return plt

def plot_power_spectrum(waves, sampling_rate):

    pwr_spec0 = power_spectrum(generate_power_spectrum(wave0, sampling_rate), "10 Hz")
    pwr_spec1 = power_spectrum(generate_power_spectrum(wave1, sampling_rate), "20 Hz")
    pwr_spec2 = power_spectrum(generate_power_spectrum(wave2, sampling_rate), "30 Hz")
    pwr_spec3 = power_spectrum(generate_power_spectrum(wave3, sampling_rate), "40 Hz")
    pwr_spec4 = power_spectrum(generate_power_spectrum(wave4, sampling_rate), "50 Hz")
    pwr_specs = [pwr_spec0, pwr_spec1, pwr_spec2, pwr_spec3, pwr_spec4]

    for i in pwr_specs:
        plt.plot(i.freq, i.spectrum, label=i.name)
    
    plt.title("Power Spectrum")
    plt.xlabel('Frequency')
    plt.ylabel('Power')
    plt.grid(True)
    plt.legend(loc='upper right')
    plt.xlim(0, sampling_rate / 2) 
    plt.show()

def generate_power_spectrum(wave, sampling_rate):
    # Perform FFT to calculate power spectrum
    fft_result = numpy.fft.fft(wave.wave)
    freq = numpy.fft.fftfreq(len(wave.wave), 1 / sampling_rate)
    power_spectrum = numpy.abs(fft_result) ** 2

    return [power_spectrum, freq]

def plot_spectrogram(waves):
    # Spectrogram
    index = 0
    wave_combined = array.array('f')
    for i in waves:
        wave_combined.extend(i.wave[index*40:(index+1)*40])
        index += 1

    plt.specgram(wave_combined, Fs=sampling_rate, NFFT=64, noverlap=32)
    plt.title('Spectrogram of Sine Wave')
    plt.xlabel('Time (s)')
    plt.ylabel('Frequency (Hz)')
    plt.colorbar(label='Power/Frequency (dB/Hz)')
    plt.show()

    return plt

def plot_ACF(waves):
    # Compute ACF
    lag_max = 50
    acf_values_10Hz = acf(waves[0].wave, nlags=lag_max, fft=True)
    acf_values_20Hz = acf(waves[1].wave, nlags=lag_max, fft=True)

    # Plot ACF
    plot3 = plt
    plot3.stem(numpy.arange(0, lag_max+1), acf_values_10Hz)
    plot3.stem(numpy.arange(0, lag_max+1), acf_values_20Hz)
    plot3.title('Autocorrelation Function (ACF) of Sine Wave')
    plot3.xlabel('Lag')
    plot3.ylabel('Autocorrelation')
    plot3.grid(True)
    plot3.show()

def plot_PACF(waves):
    lag_max = 50
    pacf_values_10Hz = pacf(waves[0].wave, nlags=lag_max)
    pacf_values_20Hz = pacf(waves[1].wave, nlags=lag_max)

    # Plot PACF
    plot3 = plt
    plot3.stem(numpy.arange(0, lag_max+1), pacf_values_10Hz)
    plot3.stem(numpy.arange(0, lag_max+1), pacf_values_20Hz)
    plot3.title('Partial Autocorrelation Function (PACF) of Sine Wave')
    plot3.xlabel('Lag')
    plot3.ylabel('Partial Autocorrelation')
    plot3.grid(True)
    plot3.show()

# Sine wave Parameters
frequency = [10, 20, 30, 40, 50]  # Hz
amplitude = 1.0
duration = 1.0  # seconds
sampling_rate = 200  # Hz
noise = 0

# Generate sine wave
wave0 = sine_wave(generate_sine_wave(frequency[0], amplitude, duration, sampling_rate, noise), "10 Hz")
wave1 = sine_wave(generate_sine_wave(frequency[1], amplitude, duration, sampling_rate, noise), "20 Hz")
wave2 = sine_wave(generate_sine_wave(frequency[2], amplitude, duration, sampling_rate, noise), "30 Hz")
wave3 = sine_wave(generate_sine_wave(frequency[3], amplitude, duration, sampling_rate, noise), "40 Hz")
wave4 = sine_wave(generate_sine_wave(frequency[4], amplitude, duration, sampling_rate, noise), "50 Hz")
waves = [wave0, wave1, wave2, wave3, wave4]

# Line Plots
plot = plt
for i in waves:
    plot_wave(plot, i)
plot.legend(loc='upper right')
plot.show()

# Power Spectrum
plot_power_spectrum(waves, sampling_rate)

# Spectrogram
plot_spectrogram(waves)

# ACF
plot_ACF(waves)

# PACF
plot_PACF(waves)
