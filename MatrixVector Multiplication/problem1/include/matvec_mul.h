#include <stdlib.h>
#include <stdio.h>

void matvec_mul(float **restrict matrix, float *restrict vector, int size, float *restrict result);
void vecmat_mul(float **restrict matrix, float *restrict vector, int size, float *restrict result);