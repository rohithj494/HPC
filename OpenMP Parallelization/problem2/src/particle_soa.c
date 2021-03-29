#include "bitmap.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define PI 3.1415

typedef struct {
  float *x, *y, *vx, *vy;
} Coord;

int main(int argc, char* argv[])
{
  int nParticles, timesteps;
  float dt = 0.025;
  int dim = 512;
  struct timespec start, end;
  double total_time=0.0;
  char buf[50];
  int threads;
  if (argc == 1) {
        nParticles = 512;
    timesteps = 128;
    
  } else if (argc>=3){
    nParticles = atoi(argv[1]);
    timesteps = atoi(argv[2]);
  }
  else{
    printf("Not enough arguments\n");
    exit(0);
  }
  threads= atoi(argv[3]);
  RgbQuad* data = malloc(dim * dim * sizeof(RgbQuad));
  Coord pInfo;
  pInfo.x = malloc(sizeof(float) * nParticles);
  pInfo.y = malloc(sizeof(float) * nParticles);
  pInfo.vx = malloc(sizeof(float) * nParticles);
  pInfo.vy = malloc(sizeof(float) * nParticles);

  float p, q;
  srand(time(NULL));
  for (int i = 0; i < nParticles; i++) {
    p = (float)rand() / RAND_MAX * PI;
    q = (float)rand() / RAND_MAX * PI;
    pInfo.x[i] = p;
    pInfo.y[i] = q;
    pInfo.vx[i] = sin(p) * cos(q);
    pInfo.vy[i] = -cos(p) * sin(q);
  }

  // Simulation
  for (int i = 0; i < timesteps; i++) {

    // loop to set previous positions to black
    // for (int j=0; j<nParticles; j++){
    //     int pixelx, pixely;
    // 		pixelx= pInfo.x[j]/PI * dim;
    // 		pixely= pInfo.y[j]/PI * dim;

    // 		RgbQuad* pix = &data[pixelx * dim + pixely];
    // 		pix->red = 0;
    //     	pix->green = 0;
    //     	pix->blue = 0;
    // }

    // loop to set new positions
    clock_gettime(CLOCK_MONOTONIC, &start);
    #pragma omp parallel for schedule(static) shared(pInfo, nParticles, timesteps) \
        num_threads(threads)
    for (int j = 0; j < nParticles; j++) {
      pInfo.x[j] += pInfo.vx[j] * dt;
      pInfo.y[j] += pInfo.vy[j] * dt;

      // printf("%d %f %f\n", j, pInfo[j].x*dt, pInfo[j].y*dt);
      // printf("%d %f %f\n", j, pInfo[j].x, pInfo[j].y);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed=  (end.tv_sec-start.tv_sec)+ 1e-9 * (end.tv_nsec-start.tv_nsec);
    total_time+=elapsed;
    // loop to set the bitmap colours
    for (int j = 0; j < nParticles; j++) {
      int pixelx, pixely;

      if (pInfo.x[j] >= 0 && pInfo.y[j] >= 0 && pInfo.x[j] < PI && pInfo.y[j] < PI) {
      pixelx = pInfo.x[j] / PI * dim;
      pixely = pInfo.y[j] / PI * dim;

      RgbQuad* pix = &data[pixelx * dim + pixely];
      pix->red = 255;
      pix->green = 0;
      pix->blue = 0;
      }
      
    }
    int check= mkdir("bmp", 0777);

    snprintf(buf, 50, "bmp//particle_soa_%d.bmp", i);
      save_bitmap(data, dim, dim, buf);
  }
  printf("%f", total_time);

   free(data);
  free(pInfo.x);
  free(pInfo.y);
  free(pInfo.vx);
  free(pInfo.vy);
}
