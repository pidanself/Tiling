#include<stdlib.h>
#include<stdio.h>
#include "plasma.h"
#include "plasma_internal.h"

void matGene(float* A, int lda , int m , int n) {
    srand(time(NULL));
    for(int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            float temp=rand()%100;//产生0-RAND_MAX的数
            float f=(float)rand()/RAND_MAX;//产生0-1的数
            A[i * lda + j] = temp*f; //产生A[j][i]
        }
    }
}

void printDesc(plasma_desc_t a);

int main(){
    plasma_init();
    //利用plasma_desc_general_create函数生成基本的desc
    int m,n,lda;
    m=100;
    n=100;
    lda=128;
    plasma_context_t *plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("plasma not initialized");
        return PlasmaErrorNotInitialized;
    }

    // Check input arguments.
    if (m < 0) {
        plasma_error("illegal value of m");
        return -1;
    }
    if (n < 0) {
        plasma_error("illegal value of n");
        return -2;
    }
    if (lda < imax(1, m)) {
        plasma_error("illegal value of lda");
        return -4;
    }

    // quick return
    if (imin(m, n) == 0)
        return PlasmaSuccess;
    // printf("finish plasma_context_self\n");

    // Set tiling parameters.
    int nb = plasma->nb;
    plasma_desc_t A;
    int retval;
    retval = plasma_desc_general_create(PlasmaRealFloat, nb, nb,
                                        m, n, 0, 0, m, n, &A);
    //printDesc(A);
    if (retval != PlasmaSuccess) {
        plasma_error("plasma_desc_general_create() failed\n");
        return retval;
    }
    // printf("finish plasma_desc_general_create\n");
    //生成一个随机float矩阵
    float *pA=(float *)malloc(sizeof(float)*((n)*(lda)+2));
    matGene(pA,lda,m,n);
    // printf("finish matGene\n");
    //生成其余参数
    // Initialize sequence.
    plasma_sequence_t sequence;
    retval = plasma_sequence_init(&sequence);

    // Initialize request.
    plasma_request_t request;
    retval = plasma_request_init(&request);
    //printf("finish some init\n");
    //用ge2desc函数完成转换
    plasma_psge2desc(pA, lda, A, &sequence, &request);
    printDesc(A);
    return 0;
}

void printDesc(plasma_desc_t a){
    printf("'type:'%d;'uplo':%d;'precision':%d\nmb:%d;nb:%d;\ngm:%d;gn:%d;gmt:%d;gnt:%d\ni:%d;j:%d;m:%d;n:%d;mt:%d;nt:%d\nkl:%d;ku:%d;klt:%d;kut:%d\n",a.type,a.uplo,a.precision,a.mb,a.nb,a.gm,a.gn,a.gmt,a.gnt,a.i,a.j,a.m,a.n,a.mt,a.nt,a.kl,a.ku,a.klt,a.kut);
    printf("%f\n",((float*)a.matrix)[0]);
}