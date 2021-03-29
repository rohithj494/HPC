#include "bitmap.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// Produces concentric quarter-circles of different colors in demo.bmp

int main()
{
  int32_t dim = 512;
  RgbQuad* data = malloc(dim * dim * sizeof(RgbQuad));
  for (int32_t i = 0; i < dim; ++i) {
    for (int32_t j = 0; j < dim; ++j) {
      float x = (float)i / dim;
      float y = (float)j / dim;
      float radius = sqrtf(x * x + y * y);

      RgbQuad* pix = &data[i * dim + j];
      if (radius > 1.0) {
        pix->red = 0;
        pix->green = 0;
        pix->blue = 0;
      } else if (radius > 0.75) {
        pix->red = 255;
        pix->green = 255;
        pix->blue = 255;
      } else if (radius > 0.5) {
        pix->red = 255;
        pix->green = 0;
        pix->blue = 0;
      } else if (radius > 0.25) {
        pix->red = 0;
        pix->green = 255;
        pix->blue = 0;
      } else {
        pix->red = 0;
        pix->green = 0;
        pix->blue = 255;
      }
    }
  }
  save_bitmap(data, dim, dim, "demo.bmp");
  free(data);
}
