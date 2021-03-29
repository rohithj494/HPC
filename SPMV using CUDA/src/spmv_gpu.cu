#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct
{
	int *data;
	int *transposed;
	int *col_index;
	int *transposed_index;
	size_t m;
	size_t n;
	size_t row_len;
	size_t data_len;
} EllMatrix;


//Kernel function for spmv
__global__ void ell_spmv(int *transposed, int *transposed_index, int *vectorx, int *vectory, int n, int cols){
    
    for (int i = threadIdx.x + blockIdx.x * blockDim.x; i < n; i += blockDim.x * gridDim.x)
   {
    int sum = 0;
    for (int j = 0; j < cols; j++)
    {
        sum += transposed[j * n + i] * vectorx[transposed_index[j * n + i]];
    }
    vectory[i] += sum;
   }
}


int main(int argc, char *argv[])
{

	if (argc < 5){
		printf ("Too few arguments. Exiting.\n");
		exit(0);
	}

	int n = atoi(argv[1]);
	float p = atof(argv[2]);
    int gridDim= atoi(argv[3]);
    int blockDim= atoi(argv[4]);

    //Reading input data
	char Abuf[50];
	char xbuf[50];
	char ybuf[50];

	snprintf(Abuf, 50, "A_%d.dat", n);
	snprintf(xbuf, 50, "x_%d.dat", n);
	snprintf(ybuf, 50, "y_%d.dat", n);

	FILE *Afile = fopen(Abuf, "rb");
	FILE *xfile = fopen(xbuf, "rb");
	FILE *yfile = fopen(ybuf , "rb");

	int32_t *fullMatrix = (int32_t *)malloc(n * n * sizeof(int32_t));
	int32_t *vectorx = (int32_t *)malloc(n * sizeof(int32_t));
	int32_t *vectory = (int32_t *)malloc(n * sizeof(int32_t));

	size_t count = fread(fullMatrix, sizeof(int32_t), n * n, Afile);
	count= fread(vectorx, sizeof(int32_t), n, xfile);
	count= fread(vectory, sizeof(int32_t), n, yfile);

	//Setting the sparse matrix
	EllMatrix *sparseStruct = (EllMatrix *)malloc(sizeof(EllMatrix));
	sparseStruct->m = n;
	sparseStruct->n = n;
	int maxRowLen;

	//Finding max row length
	for (int i = 0; i < n; i++)
	{
		maxRowLen = 0;
		for (int j = 0; j < n; j++)
		{
			if (fullMatrix[i * n + j] != 0)
			{
				maxRowLen++;
			}
		}
		if (maxRowLen >= sparseStruct->row_len)
		{
			sparseStruct->row_len = maxRowLen;
		}
	}

	//allocating space for eLL matrix
	sparseStruct->data_len = sparseStruct->row_len * n;
	sparseStruct->data = (int *)malloc(sizeof(int) * sparseStruct->data_len);
	sparseStruct->col_index = (int *)malloc(sizeof(int) * sparseStruct->data_len);
	sparseStruct->transposed = (int *)malloc(sizeof(int) * sparseStruct->data_len);
	sparseStruct->transposed_index= (int *)malloc(sizeof(int) * sparseStruct->data_len);

	//Setting up the ELL matrix
	int k = 0;
	for (int i = 0; i < n; i++)
	{
		int rowContent = 0;
		for (int j = 0; j < n; j++)
		{
			if (fullMatrix[(i * n) + j] != 0)
			{
				sparseStruct->data[k] = fullMatrix[i * n + j];
				sparseStruct->col_index[k] = j;
				++k;
				++rowContent;
			}
		}

		//Padding with zeros
		while (rowContent < sparseStruct->row_len)
		{
			sparseStruct->col_index[k] = 0;
			sparseStruct->data[k] = 0;
			++k;
			++rowContent;
		}
	}

	//Transposing
	k = sparseStruct->row_len;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < sparseStruct->row_len; j++)
		{
			sparseStruct->transposed[j * n + i] = sparseStruct->data[i * k + j];
			sparseStruct->transposed_index[j*n+i]= sparseStruct->col_index[i*k+j];
		}
	}

    //Setting up Kernel variables
    int *d_transposed, *d_transposed_index, *d_vectorx, *d_vectory;
    cudaMalloc((void **)&d_transposed,sizeof(int32_t) * sparseStruct->data_len);
    cudaMalloc((void **)&d_transposed_index, sizeof(int32_t) * sparseStruct->data_len);
    cudaMalloc((void **)&d_vectorx, sizeof(int32_t) * n);
    cudaMalloc((void **)&d_vectory, sizeof(int32_t)*n);

	cudaMemcpy(d_transposed, sparseStruct->transposed, sizeof(int32_t) * sparseStruct->data_len, cudaMemcpyHostToDevice);
	cudaMemcpy(d_transposed_index, sparseStruct->transposed_index, sizeof(int32_t) * sparseStruct->data_len, cudaMemcpyHostToDevice);
	cudaMemcpy(d_vectorx, vectorx, sizeof(int32_t) * n, cudaMemcpyHostToDevice);
	cudaMemcpy(d_vectory, vectory, sizeof(int32_t) * n, cudaMemcpyHostToDevice);

    cudaEvent_t tick, tock;
	cudaEventCreate(&tick);
	cudaEventCreate(&tock);

	cudaEventRecord(tick, 0);
	ell_spmv<<<gridDim, blockDim>>>(d_transposed, d_transposed_index, d_vectorx, d_vectory, n, sparseStruct->row_len);
	cudaEventRecord(tock, 0);
	cudaEventSynchronize(tock);
	
	float time;
	cudaEventElapsedTime(&time, tick, tock);
	cudaMemcpy(vectory, d_vectory, sizeof(int32_t) * n, cudaMemcpyDeviceToHost);

	int *multVect = (int *)calloc(n, sizeof(int));
	char multBuff[50];
	snprintf(multBuff, 50, "Axy_%d.dat", n);
	FILE *multFile = fopen(multBuff, "rb");
	count = fread(multVect, sizeof(int), n, multFile);

	//Checking if final answer is correct
	for (int j = 0; j < n; j++)
	{
		if (multVect[j]!=vectory[j]){
			printf("Error!");
			exit(0);
		}
	}
	printf("Test pass\n");
	printf("Time taken= %f ms\n", time);

	free(fullMatrix);
	free(vectorx);
	free(vectory);
	free(sparseStruct->data);
	free(sparseStruct->col_index);
	free(sparseStruct->transposed_index);
	free(sparseStruct->transposed);
	free(sparseStruct);
	free(multVect);
	fclose(Afile);
	fclose(xfile);
	fclose(yfile);
	fclose(multFile);
	cudaFree(d_transposed);
	cudaFree(d_transposed_index);
	cudaFree(d_vectory);
	cudaFree(d_vectorx);

}