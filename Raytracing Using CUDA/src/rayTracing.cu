extern "C"
{
#include "bitmap.h"
}

#include "helper_cuda.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

__device__ float vecMod(float *vec)
{
	return sqrtf(pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2));
}

__device__ float vecDot(float *v1, float *v2)
{
	float dot = (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
	return dot;
}

__global__ void rayTrace(RgbQuad *data, int n)
{

	int Wmax = 10;
	float L[3] = {4, 4, -1};
	float C[3] = {0, 12, 0};
	int R = 6;
	float delX = (2 * Wmax) / (float)n;
	float delZ = delX;

	for (int i = (blockIdx.x * blockDim.x) + threadIdx.x; i < n;
		 i += (gridDim.x * blockDim.x))
	{
		for (int j = (blockIdx.y * blockDim.y) + threadIdx.y; j < n;
			 j += (gridDim.y * blockDim.y))
		{

			float b;
			float Wx = (i * delX) - Wmax;
			float Wz = (j * delZ) - Wmax;

			float D = pow(R, 2) - pow((Wx - C[0]), 2) - pow((Wz - C[2]), 2);
			if (D < 0.0)
				b = 0.0;
			else
			{
				//float Iy = C[1] - sqrtf(D);
				float I[3] = {Wx, (C[1] - sqrtf(D)), Wz};
				//float tempN[3] = {I[0] - C[0], I[1] - C[1], I[2] - C[2]};
				float N[3] = {(I[0] - C[0]), (I[1] - C[1]), (I[2] - C[2])};
				float mod = vecMod(N);
				N[0]= N[0]/mod; N[1]=N[1]/mod; N[2]=N[2]/mod;
				//N[3] = {N[0]/ mod, N[1] / mod, N[2] / mod};
				//float tempS[3] = {L[0] - I[0], L[1] - I[1], L[2] - I[2]};
				float S[3] = {(L[0] - I[0]) , (L[1] - I[1]), (L[2] - I[2]) };
				mod = vecMod(S);
				S[0]= S[0]/mod; S[1]=S[1]/mod; S[2]=S[2]/mod;
				//float S[3] = {(L[0] - I[0]) / mod, (L[1] - I[1]) / mod, (L[2] - I[2]) / mod};

				float dot = vecDot(S, N);
				if (dot <= 0)
					b = 0.0;
				else
				{
					b = dot;
					RgbQuad *pix = &data[i * n + j];
					pix->red = (int)(b * 255);
					pix->green = (int)(b * 255);
					pix->blue = (int)(b * 255);
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	cudaEvent_t tick, tock;
	cudaEventCreate(&tick);
	cudaEventCreate(&tock);
	int gridX, gridY, blockX, blockY;
	gridX=atoi(argv[2]); gridY=atoi(argv[3]); blockX=atoi(argv[4]); blockY=atoi(argv[5]);
	int n = atoi(argv[1]);
	RgbQuad *data = (RgbQuad *)malloc(n * n * sizeof(RgbQuad));

	RgbQuad *d_img;
	cudaMalloc((void **)&d_img, n * n * sizeof(RgbQuad));
	cudaMemcpy(d_img, data, n * n * sizeof(RgbQuad), cudaMemcpyHostToDevice);

	dim3 blockDim(blockX, blockY, 1);
	dim3 gridDim(gridX, gridY, 1);
	
	cudaEventRecord(tick, 0);
	rayTrace<<<gridDim, blockDim>>>(d_img, n);
	cudaEventRecord(tock, 0);

	cudaEventSynchronize(tock);

	float time;
	cudaEventElapsedTime(&time, tick, tock);

	printf("Time taken= %f ms", time);
	cudaMemcpy(data, d_img, n * n * sizeof(RgbQuad), cudaMemcpyDeviceToHost);
	save_bitmap(data, n, n, "demo.bmp");

	free(data);
	cudaEventDestroy(tick);
	cudaEventDestroy(tock);
	cudaFree(d_img);
}
