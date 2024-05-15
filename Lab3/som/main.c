/*
 * main.c
 *
 *  Created on: 14-May-2024
 *      Author: shawn
 */




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <float.h>

// Data sets found in data.h
#include "data.h"

// IRIS data set to be used
#ifdef IRIS_DATA

#define N_SAMPLES	150	// Rows
#define M_FEATURES	4	// Columns	// Dimensions of grid weights
#define SOM_HEIGHT		8	// Output grid height
#define SOM_WIDTH		8	// Output grid width

// BME data set to be used
#elif BME_DATA

#define N_SAMPLES	180	// Rows
#define M_FEATURES	128	// Columns	// Dimensions of grid weights
#define SOM_HEIGHT		8	// Output grid height
#define SOM_WIDTH		8	// Output grid width

#else

#define N_SAMPLES	10	// Rows
#define M_FEATURES	3	// Columns	// Dimensions of grid weights
#define SOM_HEIGHT		2	// Output grid height
#define SOM_WIDTH		2	// Output grid width

// For testing algorithm only
const double test_data[] =		//data is formatted as feature1, 2, 3
{30.3,30.5,30.1,
		30,30.9,30.1,
		30.6,30,30.1,
		30.9,30.2,30.1,
		15.2,15.9,15.1,
		15.3,15.9,15,
		15.1,15.5,15.1,
		45.3,45.6,45.1,
		45.1,45.8,45.2,
		45.6,45.1,45.7,
		};
//Neurons should match around 15, 30 and 45

#endif

#define LEARNING_RATE	0.1	// Learning rate
#define WEIGHT_UPDATE_RATE	fmax(SOM_HEIGHT,SOM_WIDTH)/2.0	// Weight update rate
#define MAX_ITER	1000	// Maximum iterations


// Data structure to represent a neuron
typedef struct Neuron {
	double weights[M_FEATURES];
} neuron;

// Data structure to represent the SOM
typedef struct SOM {
	neuron neurons[SOM_WIDTH][SOM_HEIGHT];
} som_grid;

// Position of neurons in grid
typedef struct {
	int x;
	int y;
} t_pos;

double euclidean_distance(double* x, double* y, int m_features){

	double sum = 0;
	for(int i = 0; i < m_features; i++){
		sum += pow((x[i] - y[i]),2);
	}
	return sqrt(sum);
}

int random_number(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}


void SOM(t_pos *assignment, double *data, int n_samples, int m_features, int height, int width, int max_iter, float lr, float sigma){

	// Initialise 2D grid along with weights of neurons with random values
	som_grid grid;
	memset(&grid, 0, sizeof(grid));

	for(int row = 0; row < height; row++){
		for(int col = 0; col < width; col++){
			for(int weight = 0; weight < m_features; weight++)
				grid.neurons[row][col].weights[weight] = random_number(0,1000);
		}
	}

	//Iterate for max_iter iterations
	for(int iter = 0; iter < max_iter; iter++){

		// Calculate learning rate and sigma for current iteration
		double sigma_curr = sigma * exp(-(double)iter / (double)max_iter);
		double learning_rate_curr = lr * exp(-(double)iter / (double)max_iter);

		// Find BMU
		{
			// iterate through all samples
			for(int sample = 0; sample < n_samples; sample++){
				// Pick random min distance
				//double min_distance = DBL_MAX;
				double min_distance = euclidean_distance(&data[sample*m_features],
								&grid.neurons[0][0].weights[0], m_features);
				// traverse grid of neurons
				for(int row = 0; row < height; row++){
					for(int col = 0; col < width; col++){
						// Calculate euclidean distance between minimum datasets and weights
						double distance = euclidean_distance(&data[sample * m_features],
								&(grid.neurons[row][col].weights[0]), m_features);
						// assign sample to centroid with least distance
						if(distance < min_distance){
							min_distance = distance;
							// Assign sample to current neuron
							assignment[sample].x = row;
							assignment[sample].y = col;
						}
					}
				}

			}
		}

		// Update weights of som neurons using learning rate, bmu and neighbourhood function
		{
			// iterate through all samples
			for(int sample = 0; sample < n_samples; sample++){
				// iterate through grid
				for(int row = 0; row < height; row++){
					for(int col = 0; col < width; col++){
						// Calculate Update distribution for current neuron
						double dist_x = (double) (row - assignment[sample].x);
						double dist_y = (double) (col - assignment[sample].y);
						double distance = pow(dist_x,2) + pow(dist_y,2);
						double update_distribution = exp(-(distance / (2.0 * pow(sigma_curr,2))));

						// Update current neuron weights
						for(int weight = 0; weight < m_features; weight++)
							grid.neurons[row][col].weights[weight] += learning_rate_curr * update_distribution *
							(data[sample * m_features + weight] - grid.neurons[row][col].weights[weight]);

					}
				}
			}
		}
	}
	// iterate through grid
	printf("Neuron weights: ");
	for(int row = 0; row < height; row++){
		for(int col = 0; col < width; col++){
			printf("\nNeuron (%d,%d): ",row, col);
			for(int weight = 0; weight < m_features; weight++){
				printf("%f ",grid.neurons[row][col].weights[weight]);
			}

		}
	}
	printf("\n");
}

int main(void){

	// Initialize assignment values with 0 for test
	t_pos assignment[N_SAMPLES] = {0};		// Which cluster its assigned to
	double data[N_SAMPLES * M_FEATURES] = {0};			// Input from file
	// Of the form: sample0_feature0, sample0_feature1,... samplen_feature0, samplen_feature1...

	// Copy correct dataset to data array
#ifdef IRIS_DATA
	memcpy(data, IRIS, sizeof(IRIS));
#elif BME_DATA
	memcpy(data, BME, sizeof(BME));
#else
	memcpy(data, test_data, sizeof(test_data));		// Copy test data from constant array
#endif
	//call SOM function
	SOM(assignment, data, N_SAMPLES, M_FEATURES, SOM_HEIGHT, SOM_WIDTH, MAX_ITER, LEARNING_RATE, WEIGHT_UPDATE_RATE);

	//print results
	for(int n = 0; n < N_SAMPLES; n++){
		printf("%d\t sample assigned (%d,%d)\n", n, assignment[n].x, assignment[n].y);
	}


	return 1;
}
