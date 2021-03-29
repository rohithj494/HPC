#include "matvec_mul.h"


int main(int argc, char *argv[]){
    const int size= 3;
    float **matrix= malloc(sizeof(float *)* size);
    float *index= malloc(sizeof(float)* (size *size));
    
    //{7, 8, 9, 10, 11, 12, 13, 14, 15};
    for (int i=0; i<(size*size); i++){
        index[i]=i+7; 
    }
    float vector[]= {1.0, 2.0, 3.0};
    float resultxA[3]={0};
    float resultAx[3]={0};


    for (int i=0; i<size; i++){
        matrix[i]= &(index[i*size]);
    }
    
    matvec_mul(matrix, vector, size, resultAx);
    vecmat_mul(matrix, vector, size, resultxA);

    printf("Matrix= \n");
    for (int i=0; i<size; i++){
        for (int j=0; j<size; j++){
            printf ("%f ", matrix[i][j]);
        }
        printf ("\n");
    }

    printf("Vector= \n");
    for (int i=0; i<size; i++){
        printf("%f ", vector[i]);
    }
    
    printf("\nColumn vector results= \n");
    for (int i=0; i<size; i++){
        printf("%f ", resultAx[i]);
    }

    printf("\nRow vector results= \n");
    for (int i=0; i<size; i++){
        printf("%f ", resultxA[i]);
    }

    
    free(index);
    free(matrix);
}