#include "include/hw.h"
#include<stdlib.h>
#include<stdio.h>
#include "plasma.h"

//生成随机矩阵
void matGene(float* A, int lda , int m , int n);

//没理解如何访问tiling转换后的矩阵，目前只是简单的将desc中的参数以及matrix输出查看
void printDescH(hw_desc_t a);
void printDescP(plasma_desc_t a);

//没理解如何访问tiling转换后的矩阵，目前只是简单的将desc中的参数以及matrix中的数一一做比较
int compareDesc(hw_desc_t a,plasma_desc_t b);

//封装使用ge2desc的过程，便于测试
int fz_hw_ge2desc(int m,int n,int lda,hw_desc_t *A,float *pA);
int fz_plasma_psge2desc(int m,int n,int lda,plasma_desc_t *B,float *pA);

int main(){
    //利用hw_desc_general_create函数生成基本的desc
    int m,n,lda;
    m=100;
    n=100;
    lda=128;
    // Set tiling parameters.
    hw_desc_t A;
    plasma_desc_t B;
    
    //生成一个随机float矩阵
    float *pA=(float *)malloc(sizeof(float)*((n)*(lda)+2));
    matGene(pA,lda,m,n);
    //使用封装hw函数
    retval=fz_hw_ge2desc(m,n,lda,&A,pA);
    if(retval!=1){
        return -1;
    }
    //

    retval=fz_plasma_psge2desc(m,n,lda,&B,pA);
    int retval=fz_hw_ge2desc(m,n,lda,&A,pA);
    if(retval!=1){
        return -1;
    }

    printDescH(A);
    printDescP(B);

    //比较两个矩阵是否相同
    if(compareDesc(A,B)){
        printf("success!!!\n");
    }

    return 0;
}


void printDescH(hw_desc_t a){
    printf("'type:'%d;'uplo':%d;'precision':%d\nmb:%d;nb:%d;\ngm:%d;gn:%d;gmt:%d;gnt:%d\ni:%d;j:%d;m:%d;n:%d;mt:%d;nt:%d\nkl:%d;ku:%d;klt:%d;kut:%d\n",a.type,a.uplo,a.precision,a.mb,a.nb,a.gm,a.gn,a.gmt,a.gnt,a.i,a.j,a.m,a.n,a.mt,a.nt,a.kl,a.ku,a.klt,a.kut);
    printf("%f\n",((float*)a.matrix)[0]);
}

void printDescP(plasma_desc_t a){
    printf("'type:'%d;'uplo':%d;'precision':%d\nmb:%d;nb:%d;\ngm:%d;gn:%d;gmt:%d;gnt:%d\ni:%d;j:%d;m:%d;n:%d;mt:%d;nt:%d\nkl:%d;ku:%d;klt:%d;kut:%d\n",a.type,a.uplo,a.precision,a.mb,a.nb,a.gm,a.gn,a.gmt,a.gnt,a.i,a.j,a.m,a.n,a.mt,a.nt,a.kl,a.ku,a.klt,a.kut);
    printf("%f\n",((float*)a.matrix)[0]);
}

int compareDesc(hw_desc_t a,plasma_desc_t b){
    if(a.type==b.type&&a.precision==b.precision
    &&a.mb==b.mb&&a.nb==b.nb
    &&a.gm==b.gm&&a.gn==b.gn&&a.gmt==b.gmt&&a.gnt==b.gnt
    &&a.i==b.i&&a.j==b.j&&a.m==b.m&&a.n==b.n&&a.mt==b.mt&&a.nt==b.nt
    &&a.kl==b.kl&&a.ku==b.ku&&a.klt==b.klt&&a.kut==b.kut){
        printf("参数比较正常\n");
        //比较matrix中的每一个值
        int sizeDesc=a.gm*a.gn;
        float *am=(float*)a.matrix;
        float *bm=(float*)b.matrix;
        for(int i=0;i<sizeDesc;i++){
            if(am[i]!=bm[i]){
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

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

int fz_hw_ge2desc(int m,int n,int lda,hw_desc_t *A,float *pA){
    hw_init();
    hw_context_t *hw = hw_context_self();
    if (hw == NULL) {
        hw_fatal_error("hw not initialized");
        return hwErrorNotInitialized;
    }
    if (m < 0) {
        hw_fatal_error("illegal value of m");
        return -3;
    }
    if (n < 0) {
        hw_fatal_error("illegal value of n");
        return -4;
    }
    if(lda<imax(1,m)){
        hw_fatal_error("illegal value of lda");
        return -5;
    }
    int nb = hw->nb;
    int retval;
    retval = hw_desc_general_create(hwRealFloat, nb, nb,
                                        m, n, 0, 0, m, n, A);
    //printDesc(A);
    if (retval != hwSuccess) {
        hw_error("hw_desc_general_create() failed\n");
        return retval;
    }
    //生成其余参数
    // Initialize sequence.
    hw_sequence_t sequence;
    retval = hw_sequence_init(&sequence);

    // Initialize request.
    hw_request_t request;
    retval = hw_request_init(&request);
    //用ge2desc函数完成转换
    hw_ge2desc(pA, lda, *A, &sequence, &request);
    return 1;
}

int fz_plasma_psge2desc(int m,int n,int lda,plasma_desc_t *B,float *pA){
    plasma_init();
    plasma_context_t *plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("plasma not initialized");
        return PlasmaErrorNotInitialized;
    }
    int nb = plasma->nb;
    
    int retval = plasma_desc_general_create(PlasmaRealFloat, nb, nb,
                                        m, n, 0, 0, m, n, B);
    //printDesc(A);
    if (retval != PlasmaSuccess) {
        plasma_error("plasma_desc_general_create() failed\n");
        return retval;
    }
    plasma_sequence_t sequence1;
    retval = plasma_sequence_init(&sequence1);

    // Initialize request.
    plasma_request_t request1;
    retval = plasma_request_init(&request1);
    //printf("finish some init\n");
    //用ge2desc函数完成转换
    plasma_psge2desc(pA, lda, *B, &sequence1, &request1);
    return 1;
}