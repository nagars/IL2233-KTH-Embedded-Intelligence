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

#define N_SAMPLES	10	// Rows
#define M_FEATURES	2//3	// Columns	// Dimensions of grid weights
#define SOM_HEIGHT		2	// Output grid height
#define SOM_WIDTH		2	// Output grid width
#define LEARNING_RATE	0.1	// Learning rate
#define WEIGHT_UPDATE_RATE	fmax(SOM_HEIGHT,SOM_WIDTH)/2.0	// Weight update rate
#define MAX_ITER	1000	// Maximum iterations


// For testing algorithm only
/*const double test_data[] =		//data is formatted as feature1, 2, 3
{12.3,14.5,1.1,
		1.2,13.9,12.1,
		14.3,0.6,13.1,
		9.5,10.9,0.1,
		2.3,9.9,11,
		10.1,0.8,10.2,
		13.6,14.8,1,
		2.1,10.9,11,
		14.6,2.1,15.7,
		13.9,14.2,0.1};
*/
// End result should be 3 clusters (High feature 1,2 and low feature 3), (Low feature 1 and high 2,3),
//(low feature 2, high 1,3)

double test_data[20] = {1, 1, 2, 2, 3, 3, 4, 4, 4, 4, 7, 7, 7, 7, 8, 8, 9, 9, 10, 10};
// The result should be [(0, 0), (0, 0), (0, 0), (0, 0), (0, 0), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1)]

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
				double min_distance = DBL_MAX;
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
							(data[sample * m_features] - grid.neurons[row][col].weights[weight]);

					}
				}
			}
		}
	}
}


int main(void){

	// Initialize assignment values with 0 for test
	t_pos assignment[N_SAMPLES] = {0};		// Which cluster its assigned to
	double data[N_SAMPLES * M_FEATURES] = {0};			// Input from file
	// Of the form: sample0_feature0, sample0_feature1,... samplen_feature0, samplen_feature1...

	memcpy(data, test_data, sizeof(test_data));		// Copy test data from constant array

	//call SOM function
	SOM(assignment, data, N_SAMPLES, M_FEATURES, SOM_HEIGHT, SOM_WIDTH, MAX_ITER, LEARNING_RATE, WEIGHT_UPDATE_RATE);

	//print results
	for(int n = 0; n < N_SAMPLES; n++){
		printf("%d\t sample assigned (%d,%d)\n", n, assignment[n].x, assignment[n].y);
	}


	return 1;
}
