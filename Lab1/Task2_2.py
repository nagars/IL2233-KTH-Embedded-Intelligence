'''
Task: We will analyze these data to determine what (if any) activity is evoked (so
called Event-related potential, ERP) following two different types of stimuli presented to
a human subject. Specifically,
5
1. Visualize the response, i.e., ERP of the EEG, in the two conditions, A and B.
2. Find the brain activity frequency in the data of condition A (see below for condition
A and B).
'''

from scipy.io import loadmat # Import function to read data.
import matplotlib.pyplot as plt
import math
import numpy

def plot_ERP_confidence_intervals(wave, title):
    plt.plot(t, wave.mean(0))
    plt.plot(t, wave.mean(0) + 2*wave.std(0)/math.sqrt(len(wave)), 'k:', lw=1 )
    plt.plot(t, wave.mean(0) - 2*wave.std(0)/math.sqrt(len(wave)), 'k:', lw=1 )
    plt.xlabel('Time [s]')           # Label the axes
    plt.ylabel('Voltage [$\mu V$]')
    plt.title(title) 
    plt.autoscale(tight=True)        # no white-space in plot
    plt.show()

def plot_ERP(wave, title):
    plt.plot(t, wave.mean(0))        # Plot the ERP of condition A
    plt.xlabel('Time [s]')           # Label the axes
    plt.ylabel('Voltage [$\mu V$]')
    plt.title(title) 
    plt.autoscale(tight=True)        # no white-space in plot
    plt.show()

def plot_EEG_visual(wave, title):
    plt.imshow(EEGa,                                   # Image the data from condition A.
           cmap='BuPu',                            # ... set the colormap (optional)
           extent=[t[0], t[-1], 1, len(wave)],       # ... set axis limits (t[-1] represents the last element of t)
           aspect='auto',                          # ... set aspect ratio 
           origin='lower')                         # ... put origin in lower left corner
    plt.xlabel('Time[s]')                              # Label the axes
    plt.ylabel('Trial #')
    plt.title(title) 
    plt.colorbar()                                     # Show voltage to color mapping
    plt.vlines(0.25, 1, 1000, 'k', lw=2)               # Indicate stimulus onset with line
    plt.show()


def plot_power_frequency(wave, sampling_rate, title):
    
    fft_result = numpy.fft.rfft(wave.mean(0))
    freqs = numpy.fft.rfftfreq(len(wave.mean(0)), 1/sampling_rate)
    psd = numpy.abs(fft_result)**2

    plt.xlim(0, 15)
    plt.plot(freqs, psd)
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Power')
    plt.title('Power Spectrum Density (PSD)')
    plt.grid(True)
    plt.show()


data = loadmat('02_EEG-1.mat')
data.keys()
EEGa = data['EEGa']
EEGb = data['EEGb']
t = data['t'][0]
sampling_rate = 500

# Visualise EEG data
#plot_EEG_visual(EEGa, "EEGA - Trials vs time")
#plot_EEG_visual(EEGa, "EEGB - Trials vs time")

# ERP
#plot_ERP(EEGa, "ERP A - Trial 1")
#plot_ERP(EEGb, "ERP B - Trial 1")

# ERP with confidence intervals
#plot_ERP_confidence_intervals(EEGa, "ERP A w/ confidence intervals - Trial 1")
#plot_ERP_confidence_intervals(EEGb, "ERP B w/ confidence intervals - Trial 1")

# Power vs Frequency
plot_power_frequency(EEGa, sampling_rate, "EEGa - Power vs Frequency")

