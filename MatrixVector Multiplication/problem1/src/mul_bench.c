#include "matvec_mul.h"
#include <time.h>

int main (int argc, char *argv[]){
    int size= atoi(argv[1]);
    printf("%d\n", size);
    struct timespec start, end;

    float **matrix= malloc(sizeof(float *)* size);
    float *index= malloc(sizeof(float)* (size *size));
    float *vector= malloc(sizeof(float)* size);

    float *resultAx= malloc(sizeof(float)* size);
    float *resultxA= malloc(sizeof(float)* size);

    for (int i=0; i<size; i++){
        matrix[i]= &(index[i*size]);
    }

    for (int i=0; i<size; i++){
        vector[i]=0.0;
        resultAx[i]=0.0;
        resultxA[i]=0.0;
        for (int j=0; j<size; j++){
            matrix[i][j]=0.0;
        }
    }

    for (int i=0; i<5; i++){
        clock_gettime(CLOCK_MONOTONIC, &start);
        matvec_mul(matrix, vector, size, resultAx);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed_seconds= (end.tv_sec-start.tv_sec)+ 1e-9 * (end.tv_nsec-start.tv_nsec);
        printf("result[0] = %f\n", resultAx[0]);
        printf("elapsed time: elapsed time: %0.6f s\n",elapsed_seconds);
    }
    printf("-------------------------------------------------------------\n");
    for (int i=0; i<5; i++){
        clock_gettime(CLOCK_MONOTONIC, &start);
        vecmat_mul(matrix, vector, size, resultxA);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed_seconds= (end.tv_sec-start.tv_sec)+ 1e-9 * (end.tv_nsec-start.tv_nsec);
        printf("result[0] = %f\n", resultxA[0]);
        printf("elapsed time: elapsed time: %0.6f s\n",elapsed_seconds);
    }


    free(resultAx);
    free(resultxA);
    free(vector);
    free(index);
    free(matrix);
}