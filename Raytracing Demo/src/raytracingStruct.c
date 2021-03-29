#include<math.h>
#include<stdlib.h>
#include <time.h>
#include "bitmap.h"

typedef struct _vector{
    float x, y, z;
} vector;

vector* vecInit( float x, float y, float z){
    vector *tempvec= malloc(sizeof(vector));
    tempvec->x= x;
    tempvec->y= y;
    tempvec->z= z;

    return tempvec;
}

vector* vecSubtract(vector* v1, vector* v2){
    vector *difference= malloc(sizeof(vector));
    difference->x= v1->x - v2->x;
    difference->y= v1->y - v2->y;
    difference->z= v1->z - v2->z;

    return difference;
}

vector* vecScalarDiv(vector *vec, float a){
    vector *div= malloc(sizeof(vector));
    div->x= vec->x/a;
    div->y= vec->y/a;
    div->z= vec->z/a;

    return div;
}

float vecMod(vector *vec){
    return sqrtf(pow(vec->x, 2)+pow(vec->y, 2)+ pow(vec->z, 2));
}

float vecDot(vector *v1, vector *v2){
    float dot= (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z);
    return dot;
}

int main (int argc, char *argv[]){
    int n= 2048;
    int Wmax= 10;
    vector *L= vecInit(4, 4, -1);
    vector *C= vecInit(0, 12, 0);
    int R= 6;
    float delX= (2*Wmax)/(float)n;
    float delZ= delX;
    float mindot= 10000.0;
    float maxdot=-10000.0;
    RgbQuad *data = malloc(n * n * sizeof(RgbQuad));
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            float b;
            float Wx= (i*delX)- Wmax;
            float Wz= (j*delZ)- Wmax;

            float D= pow(R, 2)- pow((Wx- C->x), 2)- pow((Wz-C->z), 2);
            if (D<0.0)
                b=0.0;
            else{
                float Iy= C->y- sqrtf(D);
                vector *I= vecInit(Wx, Iy, Wz);

                vector *tempN= vecSubtract(I, C);
                vector *N= vecScalarDiv(tempN, vecMod(tempN));

                vector *tempS= vecSubtract(L, I);
                vector *S= vecScalarDiv(tempS, vecMod(tempS));
                float dot= vecDot(S, N);
                if (dot <=mindot)
                    mindot=dot;
                if (dot >=maxdot)
                    maxdot= dot;
                if (dot <=0)
                    b=0.0;
                else{
                    b= dot;
                    RgbQuad *pix = &data[i * n + j];
				pix->red = (int) (b*255);
				pix->green = (int) (b*255);
				pix->blue = (int) (b*255);
                }
            free(I);
            free(N);
            free(tempN);
            free(tempS);
            free(S);  
            }
            
            
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
	double elapsed=  (end.tv_sec-start.tv_sec)+ 1e-9 * (end.tv_nsec-start.tv_nsec);
	printf ("%f= total time\n", elapsed);
    // printf( "Minimum= %f, Maximum= %f", mindot, maxdot);
    save_bitmap(data, n, n, "demo.bmp");
}


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
