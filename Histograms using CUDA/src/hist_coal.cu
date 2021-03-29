#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "helper_cuda.h"

__global__ void hist_float_kernel(float *buffer, int *counts, int numObs, int numBins, float binWidth, float rangeStart, float rangeEnd)
{
	for (int i = threadIdx.x + blockIdx.x * blockDim.x; i < numObs;
		 i += blockDim.x * gridDim.x)
	{
		if (buffer[i] < rangeEnd && buffer[i] >= rangeStart)
		{
			int bin = floor((buffer[i] - rangeStart) / binWidth);
			atomicAdd(&counts[bin], 1);
		}
	}
}

int main(int argc, char *argv[])
{
	//printf("Please provide the number of observations as the last (7th) argument");
	FILE *inputFile = fopen(argv[1], "r");

	int numBins = atoi(argv[2]);
	float rangeStart = atof(argv[3]);
	float rangeEnd = atof(argv[4]);
	int gridDim = atoi(argv[5]);
	int blockDim = atoi(argv[6]);

	//Check if long is enough
	long numObs = atol(argv[7]);

	float *buffer = (float *)malloc(sizeof(float) * numObs);
	size_t count = fread(buffer, sizeof(float), numObs, inputFile);

	float *d_buffer;
	cudaMalloc((void **)&d_buffer, sizeof(float) * numObs);
	cudaMemcpy(d_buffer, buffer, sizeof(float) * numObs, cudaMemcpyHostToDevice);

	int *d_counts;
	cudaMalloc((void **)&d_counts, sizeof(int) * numBins);
	cudaMemset(d_counts, 0, sizeof(int) * numBins);

	float binWidth = (rangeEnd - rangeStart) / numBins;

	cudaEvent_t tick, tock;
	checkCudaErrors(cudaEventCreate(&tick));
	checkCudaErrors(cudaEventCreate(&tock));

	checkCudaErrors(cudaEventRecord(tick, 0));
	hist_float_kernel<<<gridDim, blockDim>>>(d_buffer, d_counts, numObs, numBins, binWidth, rangeStart, rangeEnd);
	checkCudaErrors(cudaEventRecord(tock, 0));
	checkCudaErrors(cudaEventSynchronize(tock));
	
	float time;
	checkCudaErrors(cudaEventElapsedTime(&time, tick, tock));

	int *counts = (int *)malloc(sizeof(int) * numBins);
	cudaMemcpy(counts, d_counts, sizeof(int) * numBins, cudaMemcpyDeviceToHost);

	int num = 1;
	for (float i = rangeStart; i < rangeEnd; i += binWidth)
	{
		printf("Bin no. %d: [%f - %f). Count= %d \n", num, i, i + binWidth, counts[num - 1]);
		num++;
	}

	printf( "Time taken= %f ms\n", time);
	free(buffer);
	free(counts);

	cudaEventDestroy(tick);
	cudaEventDestroy(tock);
	cudaFree(d_counts);
	cudaFree(d_buffer);
}