#include "bitmap.h"
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
	int m = atoi(argv[1]);
	int n = atoi(argv[2]);
	int threads= atoi(argv[3]);
	// int m= 1000;
	// int n= 1000;
	float delx = 3.0 / m;
	float dely = 2.0 / n;
	float realc, imagc;
	double complex c, z;
	int in_set;
	int T = 1000;
	struct timespec start, end;

	RgbQuad *data = malloc(m * n * sizeof(RgbQuad));
clock_gettime(CLOCK_MONOTONIC, &start);
//#pragma omp parallel for private (realc,imagc,c,z,in_set) \
num_threads(threads)
	for (int i = 0; i < m; i++)
	{

#pragma omp parallel for private (realc,imagc,c,z,in_set)\
num_threads(threads)
		for (int j = 0; j < n; j++)
		{
			realc = (-2.0) + (i * delx);
			imagc = (-1.0) + (j * dely);

			c = realc + (imagc * I);
			z = 0.0 + 0.0 * I;
			in_set = 1;
			for (int t = 0; t < T; t++)
			{
				z = cpowf(z, (2.0));
				z += c;
				if (cabs(z) >= 4.0)
				{
					in_set = 0;
					break;
				}
			}
			if (in_set)
			{
				//printf("%d, %d", i, j);
				RgbQuad *pix = &data[i * n + j];
				pix->red = 255;
				pix->green = 0;
				pix->blue = 0;
			}
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	double elapsed=  (end.tv_sec-start.tv_sec)+ 1e-9 * (end.tv_nsec-start.tv_nsec);
	printf ("%f= total time\n", elapsed);
	save_bitmap(data, m, n, "demo.bmp");
}

// int save_bitmap(RgbQuad* data, int32_t width, int32_t height, const char* filename)
// {
//   uint32_t data_size = width * height * sizeof(RgbQuad);

//   BitmapFileHeader head;
//   head.type = 0x4D42; // 'BM'
//   head.size = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + data_size;
//   head.reserved1 = 0;
//   head.reserved2 = 0;
//   head.off_bits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

//   BitmapInfoHeader info;
//   info.size = sizeof(BitmapInfoHeader);
//   info.width = width;
//   info.height = height;
//   info.planes = 1;
//   info.bit_count = sizeof(RgbQuad) * 8;
//   info.compression = 0;
//   info.size_image = data_size;
//   info.x_pels_per_meter = 0;
//   info.y_pels_per_meter = 0;
//   info.clr_used = 0;
//   info.clr_important = 0;

//   FILE* fp = fopen(filename, "wb");
//   if (fp) {
//     fwrite(&head, 1, sizeof(BitmapFileHeader), fp);
//     fwrite(&info, 1, sizeof(BitmapInfoHeader), fp);
//     fwrite(data, width * height, sizeof(RgbQuad), fp);
//     fclose(fp);
//     return 0;
//   } else {
//     return 1;
//   }
// }
