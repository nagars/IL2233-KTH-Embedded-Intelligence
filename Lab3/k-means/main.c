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

#define N_SAMPLES	10	// Rows
#define M_FEATURES	3	// Columns
#define NUM_CENTROIDS	3	// Number of centroids
#define MAX_ITER	1000	// Maximum iterations


// For testing algorithm only
const double test_data[] =		//data is formatted as feature1, 2, 3
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
// End result should be 3 clusters (High feature 1,2 and low feature 3), (Low feature 1 and high 2,3),
//(low feature 2, high 1,3)

//double test_data[20] = {1, 1, 2, 2, 3, 3, 4, 4, 4, 4, 7, 7, 7, 7, 8, 8, 9, 9, 10, 10};

double euclidean_distance(double* x, double* y, int m_features){

	double sum = 0;
	for(int i = 0; i < m_features; i++){
		sum += pow((x[i] - y[i]),2);
	}
	return sqrt(sum);
}

int random_number(int min, int max) {
	//time_t t;
	//srand((unsigned)time(&t));
    return (rand() % (max - min + 1)) + min;
}


void kmeans(int *assignment, int K, int max_iter, int n_samples, int m_features, double *data){

	double* centroids = (double*)malloc(K * m_features * sizeof(double));// Actual centroids
	//double centroids[K][m_features]
	int* centroid_datapoints = (int*)malloc(K * sizeof(int));// Tracks the number of data points assigned to each centroid
	double* centroid_sums = (double*)malloc(K * m_features * sizeof(double)); // Tracks sum of all datapoints in centroid used to calculate mean
	//double centroid_sum[K][m_features]

	memset(centroids, 0, K * m_features * sizeof(double));
	memset(centroid_datapoints, 0, K * sizeof(int));
	memset(centroid_sums, 0, K * m_features * sizeof(double));

	// Set assignments to 0
	memset(assignment, 0, sizeof(int) * n_samples);

	// Initalise centroids with random data points
	for(int centroid_num = 0; centroid_num < K; centroid_num++){
		int random_index = random_number(0, n_samples - 1);
		//int random_index = i*4;
		//memcpy(centroids + i*m_features,  data + random_index * m_features, m_features);
		centroids[centroid_num*m_features] = data[random_index * m_features];
		centroids[centroid_num*m_features + 1] = data[random_index * m_features + 1];
	}


	// Iterate for max_iterations
	for(int n = 0; n < max_iter; n++){
		// iterate through all samples
		for(int sample = 0; sample < n_samples; sample++){
			double min_distance = euclidean_distance(&data[sample*m_features], centroids, m_features);
			//iterate through all centroids
			for(int centroid_num = 0; centroid_num < K; centroid_num++){
				// Calculate minimum euclidean distance of all data points to all centroids
				double distance = euclidean_distance(&data[sample*m_features], &centroids[centroid_num * m_features], m_features);
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
			for(int feature_num = 0; feature_num < m_features; feature_num++){
				centroid_sums[assignment[sample]*m_features + feature_num] += data[sample*m_features + feature_num];
			}
			// increment array tracking how many data points in each centroid
			centroid_datapoints[assignment[sample]]++;
		}


		// Update centroids by calculating mean of assigned data points
		// iterate through all centroids
		for(int centroid_num = 0; centroid_num < K; centroid_num++){
			for(int feature_num = 0; feature_num < m_features; feature_num++){
				centroids[centroid_num * m_features + feature_num] = centroid_sums[centroid_num * m_features + feature_num]
																				   /centroid_datapoints[centroid_num];
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
