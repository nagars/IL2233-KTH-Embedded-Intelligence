/*
 * main.c
 *
 *  Created on: 10-May-2024
 *      Author: shawn
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void read_data_iris(){

}

void read_data_BME(){

}

void kmeans(int *assignment, int K, int max_iter, int n_samples, int m_features, double *data){

}

int main(void){

	int n_samples, m_features;	// No. of rows, columns
	int* assignment;			// Which cluster its assigned to
	double* data;				// Input from file
	int K = 3;					// No. of clusters
	int max_iter = 100;			// Total iterations


	// Select which mode to run in: Test, IRIS data set, BME data set
	int input = 0;
	printf("Select 0 for inbuilt test data, 1 for Iris or 2 for BME data sets\n");
	scanf("%d", &input);

	if(input == 0){

		// Test sample data
		n_samples = 10;
		m_features = 3;

		// Initialize assignment values with 0 for test
		assignment = (int*)malloc(sizeof(int) * n_samples);
		data = (double*)malloc(sizeof(double) * n_samples * m_features);

		memset(assignment, 0, sizeof(assignment));		// Set assignments to 0 for testing
		memcpy(data, test_data, sizeof(test_data));		// Copy test data from constant array

	}else if (input == 1){
	//Iris
		n_samples = 100;		//temp values
		m_features = 100;

		assignment = (int*)malloc(sizeof(int) * n_samples);
		data = (double*)malloc(sizeof(double) * n_samples * m_features);

		// Read data file
		/**TO BE IMPLEMENTED**/

		// Store in double array

		// Randomly assign data points to assignments

	}else if(input == 2){
	//BME
		n_samples = 100;		//temp values
		m_features = 100;

		assignment = (int*)malloc(sizeof(int) * n_samples);
		data = (double*)malloc(sizeof(double) * n_samples * m_features);

		// Read data file
		/**TO BE IMPLEMENTED**/

		// Store in double array

		// Randomly assign data point values

	}else {
		return 0;
	}

	//call k-means function
	kmeans(assignment, K, max_iter, n_samples, m_features, data);

	//print results
	for(int n = 0; n < n_samples; n++){
		printf("%d \t sample assigned to %d\n", n, assignment[n]);
	}

	free(assignment);
	free(data);
	return 1;
}
