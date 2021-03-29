#include "matvec_mul.h"


void matvec_mul(float **restrict matrix, float *restrict vector, int size, float *restrict result){

    for (int i=0; i<size; i++){

        for (int j=0; j<size;j++){
            result[i]+= matrix[i][j] *vector[j];
        }

    }

    
}


void vecmat_mul(float **restrict matrix, float *restrict vector, int size, float *restrict result){

    for (int i=0; i<size; i++){

        for (int j=0; j<size;j++){
            result[i]+= matrix[j][i] *vector[j];
        }
    }

    
}
