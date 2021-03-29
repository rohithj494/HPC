#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "bitmap.h"

float vecMod(float *vec)
{
	return sqrtf(pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2));
}

float vecDot(float *v1, float *v2)
{
	float dot = (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
	return dot;
}

int main(int argc, char *argv[])
{
	int n = atoi(argv[1]);
	//int threads=16;
	int Wmax = 10;
	float L[3] = {4, 4, -1};
	float C[3] = {0, 12, 0};
	int R = 6;
	float delX = (2 * Wmax) / (float)n;
	float delZ = delX;
	RgbQuad *data = malloc(n * n * sizeof(RgbQuad));
	struct timespec start, end;


	for (int k=1; k<=32; k++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		#pragma omp parallel for num_threads(k)
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			float b;
			float Wx = (i * delX) - Wmax;
			float Wz = (j * delZ) - Wmax;

			float D = pow(R, 2) - pow((Wx - C[0]), 2) - pow((Wz - C[2]), 2);
			if (D < 0.0)
				b = 0.0;
			else
			{
				float Iy = C[1] - sqrtf(D);
				float I[3] = {Wx, Iy, Wz};
				float tempN[3] = {I[0] - C[0], I[1] - C[1], I[2] - C[2]};
				float mod = vecMod(tempN);
				float N[3] = {tempN[0] / mod, tempN[1] / mod, tempN[2] / mod};
				float tempS[3] = {L[0] - I[0], L[1] - I[1], L[2] - I[2]};
				mod = vecMod(tempS);
				float S[3] = {tempS[0] / mod, tempS[1] / mod, tempS[2] / mod};

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
	clock_gettime(CLOCK_MONOTONIC, &end);
	double elapsed = (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);
	printf("%f= total time for %d threads\n", elapsed, k);
	save_bitmap(data, n, n, "demo.bmp");
}
	}
	

// int save_bitmap(RgbQuad *data, int32_t width, int32_t height, const char *filename)
// {
// 	uint32_t data_size = width * height * sizeof(RgbQuad);

// 	BitmapFileHeader head;
// 	head.type = 0x4D42; // 'BM'
// 	head.size = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + data_size;
// 	head.reserved1 = 0;
// 	head.reserved2 = 0;
// 	head.off_bits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

// 	BitmapInfoHeader info;
// 	info.size = sizeof(BitmapInfoHeader);
// 	info.width = width;
// 	info.height = height;
// 	info.planes = 1;
// 	info.bit_count = sizeof(RgbQuad) * 8;
// 	info.compression = 0;
// 	info.size_image = data_size;
// 	info.x_pels_per_meter = 0;
// 	info.y_pels_per_meter = 0;
// 	info.clr_used = 0;
// 	info.clr_important = 0;

// 	FILE *fp = fopen(filename, "wb");
// 	if (fp)
// 	{
// 		fwrite(&head, 1, sizeof(BitmapFileHeader), fp);
// 		fwrite(&info, 1, sizeof(BitmapInfoHeader), fp);
// 		fwrite(data, width * height, sizeof(RgbQuad), fp);
// 		fclose(fp);
// 		return 0;
// 	}
// 	else
// 	{
// 		return 1;
// 	}
// }
