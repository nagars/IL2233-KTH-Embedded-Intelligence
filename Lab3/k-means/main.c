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
#include <time.h>

#define threshold 0.1	// Threshold below which centroids can be considered equal
#define N_SAMPLES	10	// Rows
#define M_FEATURES	2//3	// Columns
#define NUM_CENTROIDS	2	// Number of centroids
#define MAX_ITER	100	// Maximum iterations


// For testing algorithm only
/*const double test_data[] =		//data is formatted as feature1, 2, 3
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
*/
double test_data[20] = {1, 1, 2, 2, 3, 3, 4, 4, 4, 4, 7, 7, 7, 7, 8, 8, 9, 9, 10, 10};

int euclidean_distance(double* x, double* y, int m_features){

	int sum = 0;
	for(int i = 0; i < m_features; i++){
		sum += pow((x[i] - y[i]),2);
	}
	return sqrt(sum);
}

int random_number(int min, int max) {
    srand(time(NULL));
    return (rand() % (max - min + 1)) + min;
}


void kmeans(int *assignment, int K, int max_iter, int n_samples, int m_features, double *data){

	double* centroids = (double*)malloc(K * m_features * sizeof(double));// Actual centroids
	//double centroids[K][m_features]
	double* centroid_datapoints = (double*)malloc(K * sizeof(double));// Tracks the number of data points assigned to each centroid
	double* centroid_sums = (double*)malloc(K * m_features * sizeof(double)); // Tracks sum of all datapoints in centroid used to calculate mean
	//double centroid_sum[K][m_features]

	memset(centroids, 0, K * m_features * sizeof(double));
	memset(centroid_datapoints, 0, K * sizeof(double));
	memset(centroid_sums, 0, K * m_features * sizeof(double));

	// Set assignments to 0
	memset(assignment, 0, sizeof(int) * n_samples);

	// Initalise centroids with random data points
	for(int i = 0; i < K; i++){
		int random_index = random_number(0, n_samples - 1);
		//memcpy(centroids + i*m_features,  data + random_index * m_features, m_features);
		centroids[i*m_features] = data[random_index * m_features];
		centroids[i*m_features + 1] = data[random_index * m_features + 1];
	}


	// Iterate for max_iterations
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

		// Clear centroids array
		memset(centroid_sums, 0, sizeof(double)*K*m_features);
		memset(centroid_datapoints, 0, sizeof(int)*K);


	}

	free(centroids);
	free(centroid_datapoints);
	free(centroid_sums);

	return;
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
		printf("%d\t sample assigned to centroid %d\n", n, assignment[n]);
	}

	return 1;
}
