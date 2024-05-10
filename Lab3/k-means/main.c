/*
 * main.c
 *
 *  Created on: 10-May-2024
 *      Author: shawn
 */


#include <stdio.h>

void read_data_iris(){

}

void read_data_

void kmeans(int *assignment, int K, int max_iter, int n_samples, int m_features, double *data){

}

int main(void){

	int n_samples, m_features
	int* assignment;
	int* data;
	int K = 3;
	int max_iter = 100;


	int input = 0;
	printf("Select 0 for inbuilt test data, 1 for Iris or 2 for BME data sets\n");
	scanf("%d", input);

	if(input == 0){

	n_samples = 10;		//data is feature1, 2, 3
	m_features = 3;
	double data = [8.3,9.5,1.1,
			1.2,11.9,10.1,
			8.3,1.6,10.1,
			9.5,10.9,0.1,
			2.3,9.9,11,
			10.1,0.8,11.2,
			9.6,9.8,1,
			2.1,8.9,11,
			9.6,2.1,10.7,
			10.9,9.2,0.1];
	// End result should be 3 clusters (High feature1,2 and low feature 3), (Low feature1, high 2,3),
	//(low feature 2, high 1 and 3)

	}else if (input == 1){
	//Iris
	n_samples = 100;		//temp values
	m_features = 100;

	assignment = (int*)malloc(sizeof(int) * n_samples);

	// Read data file

	// Store in double array

	// Randomly assign data points to assignments

	}else if(input == 2){
	//BME
	n_samples = 100;		//temp values
	m_features = 100;

	assignment = (int*)malloc(sizeof(int) * n_samples);

	// Read data file

	// Store in double array

	// Randomly assign data point values

	}else {
		return 0;
	}

	//call k-means function

	//print results

	return 1;
}
