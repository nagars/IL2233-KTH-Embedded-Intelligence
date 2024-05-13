/*
 * main.c
 *
 *  Created on: 10-May-2024
 *      Author: shawn
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define threshold 0.1	// Threshold below which centroids can be considered equal
#define N_SAMPLES	10	// Rows
#define M_FEATURES	3	// Columns
#define NUM_CENTROIDS	3	// Number of centroids
#define MAX_ITER	100	// Maximum iterations

// For testing algorithm only
const double test_data[] =		//data is formatted as feature1, 2, 3
{8.3,9.5,1.1,
		1.2,11.9,10.1,
		8.3,1.6,10.1,
		9.5,10.9,0.1,
		2.3,9.9,11,
		10.1,0.8,11.2,
		9.6,9.8,1,
		2.1,8.9,11,
		9.6,2.1,10.7,
		10.9,9.2,0.1};
// End result should be 3 clusters (High feature 1,2 and low feature 3), (Low feature 1 and high 2,3),
//(low feature 2, high 1,3)

int euclidean_distance(double* x, double* y, int m_features){

	int sum = 0;
	for(int i = 0; i < m_features; i++){
		sum += pow((x[i] - y[i]),2);
	}
	return sqrt(sum);
}

void kmeans(int *assignment, int K, int max_iter, int n_samples, int m_features, double *data){

	double* centroids_prev = (double*)malloc(K * m_features * sizeof(double));// Stores previous values of centroids
	int cluster_complete = 0;		// Set to one if no more clustering is possible

	double* centroids = (double*)malloc(K * m_features * sizeof(double));// Actual centroids

	double* centroid_datapoints = (double*)malloc(K * sizeof(double));// Tracks the number of data points assigned to each centroid
	double* centroid_sums = (double*)malloc(K * m_features * sizeof(double)); // Tracks sum of all datapoints in centroid used to calculate mean

	// Set assignments to 0
	memset(assignment, 0, sizeof(int) * n_samples);

	// Iterate for max_iterations or until centroids stop moving
	for(int n = 0; n < max_iter; n++){
		// iterate through all samples
		for(int sample = 0; sample < n_samples; sample++){
			double min_distance = euclidean_distance(data, centroids, m_features);
			//iterate through all centroids
			for(int centroid_num = 0; centroid_num < K; centroid_num++){
				// Calculate minimum euclidean distance of all data points to all centroids
				double distance = euclidean_distance(data + sample*m_features, centroids + centroid_num * m_features, m_features);
				// assign sample to centroid with least distance
				if(distance < min_distance){
					min_distance = distance;
					assignment[sample] = centroid_num;
				}
			}
		}

		// Clear centroids array
		memset(centroid_sums, 0, sizeof(double)*K*m_features);
		memset(centroid_datapoints, 0, sizeof(int)*K);

		// iterate through all samples
		for(int sample = 0; sample < n_samples; sample++){
			// Calculate sum of features for all datapoints in a specific centroid
			for(int i = 0; i < m_features; i++){
				centroid_sums[assignment[sample]*m_features + i] += data[sample*m_features + i];
			}
			// increment array tracking how many data points in each centroid
			centroid_datapoints[assignment[sample]]++;
		}


		// Update centroids by calculating mean of assigned data points
		// iterate through all centroids
		for(int n = 0; n < K; n++){
			for(int i = 0; i < m_features; i++){
				centroids[n * m_features + i] = centroid_sums[n * m_features + i]/centroid_datapoints[n];
			}
		}

		// Check if clustering is complete by confirming if clusters
		// are still moving or not
		cluster_complete = 1;
		// iterate through centroids
		for(int i = 0; i < K; i++){
			// iterate through features
			for(int n = 0; n < m_features; n++){
				// check if centroids match enough
				if(fabs(centroids[i * m_features + n] - centroids_prev[i * m_features + n]) > threshold)
					cluster_complete = 0;
			}
		}
		// Centroids did not move. CLustering complete
		if (cluster_complete == 1)
			return;

		// Update centroid_prev
		memcpy(centroids_prev, centroids, sizeof(double)*K*m_features);
	}
}

int main(void){

	// Initialize assignment values with 0 for test
	int assignment[N_SAMPLES] = {0};		// Which cluster its assigned to
	double data[N_SAMPLES * M_FEATURES] = {0};			// Input from file
	// Of the form: sample0_feature0, sample0_feature1,... samplen_feature0, samplen_feature1...

	memcpy(data, test_data, sizeof(test_data));		// Copy test data from constant array

		//call k-means function
	kmeans(assignment, NUM_CENTROIDS, MAX_ITER, N_SAMPLES, M_FEATURES, data);

	//print results
	for(int n = 0; n < N_SAMPLES; n++){
		printf("%d \t sample assigned to %d\n", n, assignment[n]);
	}

	return 1;
}
