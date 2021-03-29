#include "bitmap.h"
#include "mpi.h"
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int TERM_TAG = INT32_MAX;

void boss_task(RgbQuad *data, int m, int n, int row_size, int n_workers)
{
	MPI_Status stat;
	int next_row = 0;
	int T = 1000;

	for (int i = 0; i < n_workers; i++)
	{
		if (next_row < m)
		{
			//printf("Sending tag %d\n", next_row);
			MPI_Send(&next_row, 1, MPI_INT, i, next_row, MPI_COMM_WORLD);
			next_row += row_size;
		}
		else
		{
			MPI_Send(&next_row, 1, MPI_INT, i, TERM_TAG, MPI_COMM_WORLD);
		}
	}

	for (int i = 0; i < ceil(m / row_size); i++)
	{
		float *row_values = malloc(sizeof(float) * row_size * n);
		MPI_Recv(row_values, row_size * n, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		//printf("Received from source %d. Row= %d\n", stat.MPI_SOURCE, stat.MPI_TAG);
		int row = stat.MPI_TAG;
		for (int i = 0; i < row_size; i++)
		{
			for (int j = 0; j < n; j++)
			{
				RgbQuad *pix = &data[(i + row) * n + j];
				pix->red = 255 * row_values[i * n + j] / T;
				pix->blue = 0.0;
				pix->green = 0.0;
			}
		}

			if (next_row < m)
			{
				MPI_Send(&next_row, 1, MPI_INT, stat.MPI_SOURCE, next_row, MPI_COMM_WORLD);
				next_row += row_size;
			}
			else
			{
				//printf ("Sending end tag to %d", stat.MPI_SOURCE);
				MPI_Send(&next_row, 1, MPI_INT, stat.MPI_SOURCE, TERM_TAG, MPI_COMM_WORLD);
			}
		free(row_values);
	}
}

void worker_task(float *local_t, int m, int n, int row_size, int boss_rank)
{
	//printf("Executing worker task\n");
	MPI_Status stat;
	int row_number;
	float delx = 3.0 / m;
	float dely = 2.0 / n;
	float realc, imagc;
	double complex c, z;
	int in_set;
	int T = 1000;

	realc = -0.7;
	imagc = 0.26;

	c = realc + (imagc * I);
	float t;

	while (1)
	{	
		//printf("Waiting for receive\n");
		MPI_Recv(&row_number, 1, MPI_INT, boss_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		//printf("Tag received= %d", stat.MPI_TAG);

		if (stat.MPI_TAG == TERM_TAG){
			//printf("Received end tag");
			break;
		}
			

		for (int i = row_number; i < row_number + row_size; i++)
		{
			for (int j = 0; j < n; j++)
			{
				z = (-1.5 + (i * delx)) + (-1.0 + (j * dely)) * I;
				t = 0.0;
				while (cabsf(z) < 4.0 && t < T)
				{
					z = cpowf(z, (2.0));
					z += c;
					t = t + 1.0;
				}
				local_t[(i - row_number) * n + j] = t;
			}
		}

		MPI_Send(local_t, n * row_size, MPI_INT, boss_rank, row_number, MPI_COMM_WORLD);
	}
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	int n_ranks, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);
	//printf("ranks= %d\n", n_ranks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// printf("rank= %d\n", rank);
	int boss_rank = n_ranks - 1;

	int m = atoi(argv[1]);
	int n = atoi(argv[2]);
	int row_size = atoi(argv[3]);

	RgbQuad *data;
	float *local_t;

	if (rank == boss_rank)
	{
		data = malloc(m * n * sizeof(RgbQuad));
	}
	else
	{	
		
		local_t = malloc(sizeof(float) * row_size * n);
	}

	if (rank == boss_rank)
	{
		boss_task(data, m, n, row_size, n_ranks - 1);
	}
	else
	{
		worker_task(local_t, m, n, row_size, boss_rank);
	}

	if (rank == boss_rank)
	{
		save_bitmap(data, m, n, "julia_dynmic.bmp");
		free(data);
	}
	else
	{
		free(local_t);
	}

	MPI_Finalize();
}