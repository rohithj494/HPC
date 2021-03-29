#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

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

//SPMV Function for CPU
void ell_spmv(EllMatrix *sparseStruct, int *vectorx, int *vectory)
{
	int n = sparseStruct->n;
	int cols = sparseStruct->row_len;

	for (int i = 0; i < n; i++)
	{
		int sum = 0;
		for (int j = 0; j < cols; j++)
		{
			sum += sparseStruct->transposed[j * n + i] * vectorx[sparseStruct->transposed_index[j * n + i]];
		}
		vectory[i] += sum;
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3){
		printf ("Too few arguments. Exiting.\n");
		exit(0);
	}
	
	int n = atoi(argv[1]);
	float p = atof(argv[2]);
	struct timespec start, end;

	char Abuf[50];
	char xbuf[50];
	char ybuf[50];

	snprintf(Abuf, 50, "A_%d.dat", n);
	snprintf(xbuf, 50, "x_%d.dat", n);
	snprintf(ybuf, 50, "y_%d.dat", n);

	FILE *Afile = fopen(Abuf, "rb");
	FILE *xfile = fopen(xbuf, "rb");
	FILE *yfile = fopen(ybuf , "rb");

	int32_t *fullMatrix = malloc(n * n * sizeof(int32_t));
	int32_t *vectorx = malloc(n * sizeof(int32_t));
	int32_t *vectory = malloc(n * sizeof(int32_t));

	size_t count = fread(fullMatrix, sizeof(int32_t), n * n, Afile);
	fread(vectorx, sizeof(int32_t), n, xfile);
	count = fread(vectory, sizeof(int32_t), n, yfile);

	//Setting the sparse matrix
	EllMatrix *sparseStruct = malloc(sizeof(EllMatrix));
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
	sparseStruct->data = malloc(sizeof(int) * sparseStruct->data_len);
	sparseStruct->col_index = malloc(sizeof(int) * sparseStruct->data_len);
	sparseStruct->transposed = malloc(sizeof(int) * sparseStruct->data_len);
	sparseStruct->transposed_index= malloc(sizeof(int) * sparseStruct->data_len);

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

	int *multVect = calloc(n, sizeof(int));
	char multBuff[50];
	snprintf(multBuff, 50, "Axy_%d.dat", n);
	FILE *multFile = fopen(multBuff, "rb");
	count = fread(multVect, sizeof(int), n, multFile);


	clock_gettime(CLOCK_MONOTONIC, &start);
	ell_spmv(sparseStruct, vectorx, vectory);
	clock_gettime(CLOCK_MONOTONIC, &end);
	double elapsed_seconds= (end.tv_sec-start.tv_sec)+ 1e-9 * (end.tv_nsec-start.tv_nsec);

	for (int j = 0; j < n; j++)
	{
		if (multVect[j]!=vectory[j]){
			printf("Error!");
			exit(0);
		}
	}
	printf("Test pass\n");
	printf("elapsed time: elapsed time: %0.6f s\n",elapsed_seconds);


	//Maybe should've written a function for this
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

}