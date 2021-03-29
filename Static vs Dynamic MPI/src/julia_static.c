#include "bitmap.h"
#include "mpi.h"
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{

	MPI_Init(&argc, &argv);
	int n_ranks;
	MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int m_global = atoi(argv[1]);
	int m_local = m_global / n_ranks;
	int n = atoi(argv[2]);

	// int m= 1000;
	// int n= 1000;
	float delx = 3.0 / m_global;
	float dely = 2.0 / n;
	float realc, imagc;
	double complex c, z;
	int in_set;
	int T = 1000;

	realc = -0.7;
	imagc = 0.26;

	c = realc + (imagc * I);
	float t;

	float *local_t = malloc(sizeof(float) * n * m_local);

	for (int i = 0; i < m_local; i++)
	{
		for (int j = 0; j < n; j++)
		{

			z = (-1.5 + ((rank * m_local + i) * delx)) + (-1.0 + (j * dely)) * I;
			t = 0.0;
			while (cabsf(z) < 4.0 && t < T)
			{
				z = cpowf(z, (2.0));
				z += c;
				t = t + 1.0;
			}
			local_t[i * n + j] = t;
		}
	}

	if (rank == 0)
	{
		RgbQuad *data = malloc(m_global * n * sizeof(RgbQuad));

		for (int i = 0; i < m_local; i++)
		{
			for (int j = 0; j < n; j++)
			{
				RgbQuad *pix = &data[(rank * m_local + i) * n + j];
				pix->red = 255 * local_t[i * n + j] / T;
				pix->blue = 0.0;
				pix->green = 0.0;
			}
		}
		for (int k = 1; k < n_ranks; k++)
		{
			MPI_Recv(local_t, m_local * n, MPI_FLOAT, k, k, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for (int i = 0; i < m_local; i++)
			{
				for (int j = 0; j < n; j++)
				{
					RgbQuad *pix = &data[(k * m_local + i) * n + j];
					pix->red = 255 * local_t[i * n + j] / T;
					pix->blue = 0.0;
					pix->green = 0.0;
				}
			}
		}
		save_bitmap(data, m_global, n, "julia_static.bmp");
		free(data);
		free(local_t);
	}
	else
	{
		MPI_Send(local_t, m_local * n, MPI_FLOAT, 0, rank, MPI_COMM_WORLD);
		free(local_t);
	}
	MPI_Finalize();
}
