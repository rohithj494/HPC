#include "bitmap.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>


#define PI 3.1415

typedef struct {
  float x, y, vx, vy;
} Coord;

int main(int argc, char* argv[])
{
  int nParticles, timesteps;
  struct timespec start, end;
  double total_time = 0.0;
  float dt = 0.025;
  int dim = 512;
  char buf[50];


  if (argc > 1) {
    nParticles = atoi(argv[1]);
    timesteps = atoi(argv[2]);
  } else if (argc==1) {
    nParticles = 512;
    timesteps = 128;
  } else{
    printf("Not enough arguments\n");
    exit(0);
  }
  
  RgbQuad* data = malloc(dim * dim * sizeof(RgbQuad));
  Coord* pInfo = malloc(sizeof(Coord) * nParticles);

  // Initial positions and velocities
  float p, q;
  srand(time(NULL));
  for (int i = 0; i < nParticles; i++) {
    p = (float)rand() / RAND_MAX * PI;
    q = (float)rand() / RAND_MAX * PI;
    pInfo[i].x = p;
    pInfo[i].y = q;
    pInfo[i].vx = sin(p) * cos(q);
    pInfo[i].vy = -cos(p) * sin(q);
  }

  // Simulation
  for (int i = 0; i < timesteps; i++) {

    // loop to set new positions
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int j = 0; j < nParticles; j++) {
      pInfo[j].x += pInfo[j].vx * dt;
      pInfo[j].y += pInfo[j].vy * dt;

      // printf("%d %f %f\n", j, pInfo[j].x*dt, pInfo[j].y*dt);
      // printf("%d %f %f\n", j, pInfo[j].x, pInfo[j].y);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    total_time += (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);

    // loop to set the bitmap colours
    for (int j = 0; j < nParticles; j++) {
      int pixelx, pixely;
      //printf("%f\n", pInfo[j].x);
      if (pInfo[j].x >= 0 && pInfo[j].y >= 0 && pInfo[j].x < PI && pInfo[j].y < PI) {
        pixelx = pInfo[j].x / PI * dim;
        pixely = pInfo[j].y / PI * dim;

        RgbQuad* pix = &data[pixelx * dim + pixely];
        pix->red = 255;
        pix->green = 0;
        pix->blue = 0;
      }
      // pInfo[j].x>0 ? pixelx= pInfo[j].x/PI * dim : pixelx= (PI+pInfo[j].x)/PI;
      
    } 
      int check= mkdir("bmp", 0777);
      snprintf(buf, 50, "bmp//particle_aos_%d.bmp", i);
      save_bitmap(data, dim, dim, buf);
    //printf("%d\n", i);
  }
  printf("Total Time= %f", total_time);

  free(data);
  free(pInfo);

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
