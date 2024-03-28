import numpy
import matplotlib.pyplot as plt

from tensorflow import keras
from keras.layers import Dense
from keras.layers import Input
from keras.models import Sequential, load_model

from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_absolute_error
from sklearn.metrics import r2_score
from sklearn.metrics import mean_squared_error

# define an MLP model to be trained on the 80% dataset of the sine wave
def create_MLP(input_train, output_train, window_size, num_layers = 1):

    num_neurons = 64
    activator_function = 'relu'

    # Create MLP model
    model = Sequential()
    model.add(Input((window_size,)))    # Input layer
    
    # Create hidden layers
    for i in range(num_layers):
        model.add(Dense(num_neurons, activation=activator_function))    #   Hidden Layers

    model.add(Dense(1))  # Output layer, no activation for regression

    # Compile the model
    model.compile(optimizer='adam', loss='mean_squared_error')

    # Train the model
    model.fit(input_train, output_train)

    model.summary()
    return model

# Create a PRM model
def create_PRM(samples_x, samples_y, degree):
    
    # Generate polynomial features
    poly_features = PolynomialFeatures(degree=degree)
    X_poly = poly_features.fit_transform(samples_x.reshape(-1, 1))

    # Create and fit the polynomial regression model
    model = LinearRegression()
    model.fit(X_poly, samples_y)

    return [model, X_poly]

def generate_sine_wave(freq, amplitude, duration, sampling_rate, noise_level):
    """
    Generate a sine wave.

    Parameters:
        freq (float): Frequency of the sine wave in Hz.
        amplitude (float): Amplitude of the sine wave.
        duration (float): Duration of the sine wave in seconds.
        sampling_rate (int): Sampling rate (number of samples per second).

    Returns:
        numpy.ndarray: The generated sine wave.
    """
    num_samples = int(duration * sampling_rate)
    time = numpy.linspace(0, duration, num_samples)
    noise = numpy.random.uniform(-noise_level, noise_level, num_samples)
    sine_wave = amplitude * numpy.sin(2 * numpy.pi * freq * time) + noise
    return [sine_wave,time]

def plot_wave(wave, title = "Sine Wave", legend = None, colour = None):

    plt.plot(wave, label=legend, color=colour)
    plt.title(title)
    plt.xlabel('Sample')
    plt.ylabel('Amplitude')
    plt.grid(True)
    plt.show()

    return plt

def compare_waves(wave_expected, wave_actual, title = "Comparison", legend1 = None, legend2 = None, colour1 = None, colour2 = None):

    # Plot the first set of data
    plt.plot(wave_actual, label=legend2, color=colour2)
    # Overlay the second set of data on the same plot
    plt.plot(wave_expected, label=legend1, color=colour1)

    # Add labels, legend, and title
    plt.xlabel('Sample')
    plt.ylabel('Amplitude')
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.show()

    return plt

# Sine wave Parameters
frequency = 5  # Hz
amplitude = 1.0
duration = 1.0  # seconds
sampling_rate = 1000  # Hz
noise_level = 0.5   # noise

# MLP Parameters
window_size = 1  # Number of past samples to predict next sample
num_hidden_layers = 3   # Number of hidden layers in the MLP model

# PRM Parameters
poly_degree = 8 # Polynomial Equation Degree

# Generate sine wave
[sine_wave, time] = generate_sine_wave(frequency, amplitude, duration, sampling_rate, noise_level)

# Create training / testing data
input_seq = []
output_seq = []
for i in range(len(sine_wave) - window_size):
    input_seq.append(sine_wave[i:i+window_size])
    output_seq.append(sine_wave[i+window_size])
input_seq = numpy.array(input_seq)
output_seq = numpy.array(output_seq)

# Split data into 80% for training, 20% for predication
split_index = round(len(input_seq)*0.8)
# training data
input_train = input_seq[:split_index]
output_train = output_seq[:split_index]
# testing data
input_predict = input_seq[split_index:]
output_predict_expected = output_seq[split_index:]

# Create MLP Model
sine_model_mlp = create_MLP(input_train, output_train, window_size, num_hidden_layers)

# Predict output values
output_predict_actual_mlp = sine_model_mlp.predict(input_predict)

# Create PRM model
sine_model_prm = create_PRM(time[split_index + 1:], input_predict, poly_degree) 

# Predict output values
output_predict_actual_prm = sine_model_prm[0].predict(sine_model_prm[1])

print("MLP")
print('R2 = ', r2_score(output_predict_actual_mlp, output_predict_expected))
print('MAE = ', mean_absolute_error(output_predict_actual_mlp, output_predict_expected))
print('MSE = ', mean_squared_error(output_predict_actual_mlp, output_predict_expected))


print("PRM")
print('R2 = ', r2_score(output_predict_actual_prm, output_predict_expected))
print('MAE = ', mean_absolute_error(output_predict_actual_prm, output_predict_expected))
print('MSE = ', mean_squared_error(output_predict_actual_prm, output_predict_expected))


# Plot sine waves
plot_wave(sine_wave, title = "original sine wave").show()
compare_waves(output_predict_expected, output_predict_actual_mlp, "MLP", "Expected", "Actual")
compare_waves(output_predict_expected, output_predict_actual_prm, "PRM", "Expected", "Actual")

