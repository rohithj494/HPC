#include "bitmap.h"

// Adapted from https://gist.github.com/guohai/5848088
int save_bitmap(RgbQuad* data, int32_t width, int32_t height, const char* filename)
{
  uint32_t data_size = width * height * sizeof(RgbQuad);

  BitmapFileHeader head;
  head.type = 0x4D42; // 'BM'
  head.size = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + data_size;
  head.reserved1 = 0;
  head.reserved2 = 0;
  head.off_bits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

  BitmapInfoHeader info;
  info.size = sizeof(BitmapInfoHeader);
  info.width = width;
  info.height = height;
  info.planes = 1;
  info.bit_count = sizeof(RgbQuad) * 8;
  info.compression = 0;
  info.size_image = data_size;
  info.x_pels_per_meter = 0;
  info.y_pels_per_meter = 0;
  info.clr_used = 0;
  info.clr_important = 0;

  FILE* fp = fopen(filename, "wb");
  if (fp) {
    fwrite(&head, 1, sizeof(BitmapFileHeader), fp);
    fwrite(&info, 1, sizeof(BitmapInfoHeader), fp);
    fwrite(data, width * height, sizeof(RgbQuad), fp);
    fclose(fp);
    return 0;
  } else {
    return 1;
  }
}
