#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "omp.h"
#define N 1000

int initialize(double * matrix);
int _initialize(double * matrix);


int main(int argc, char** argv)
{
	int i, j, k;
	double *A, *B, *Cs, *Cp;
	struct timeval start, end;
	double timeCost;
    

	A=(double *)malloc(sizeof(double)*N*N);
	B=(double *)malloc(sizeof(double)*N*N);
	Cs=(double *)malloc(sizeof(double)*N*N);
	Cp=(double *)malloc(sizeof(double)*N*N);

	initialize(A);
	initialize(B);
	_initialize(Cs);
	_initialize(Cp);

    // sequential version
//	gettimeofday(&start, NULL);
//	for(i=0; i<N; i++)
//	  for(j=0; j<N; j++)
//		for(k=0; k<N; k++)
//		  Cs[i*N+j]+=A[i*N+k]*B[k*N+j];
//	gettimeofday(&end, NULL);
//
//	timeCost=1000000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec);
//	timeCost/=1000000;
//    printf("The squential version of matrix multiplication costs %lf seconds\n", timeCost);

    //////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
    int numThreads = atoi(argv[1]);
    printf("numThreads = %d\n", numThreads);
    omp_set_num_threads(numThreads);
    
    double begin = omp_get_wtime();
    
    // please change this into a parallel version
    #pragma omp parallel for private(i,j,k) shared (Cp,A,B)
    for(i=0; i<N; i++)
        for(j=0; j<N; j++)
            for(k=0; k<N; k++)
                Cp[i*N+j]+=A[i*N+k]*B[k*N+j];

    double endTime = omp_get_wtime();

	
	printf("The parallel version of matrix multiplication costs %f seconds with %d threads\n", (endTime -begin), numThreads);
    
    //////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
	
    // verification
	for(i=0; i<N; i++)
	  for(j=0; j<N; j++) {
            if (Cs[i*N+j] - Cp[i*N+j] < 0.000001)
                continue;
            else {
                printf("ERRORS DETECTED!!!!!!!!!!!!!\n");
                return -1;
            }
      }
    printf("Verified Correctly\n");
    printf("seq:\n");
    
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            if (i == 100 && j == 100)
                printf ("\n Value of c[%d][%d]:%f\n",i,j,Cs[i*N+j]);
        }
    }
    
    printf("parallel:\n");
    
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            
            if (i == 100 && j == 100)
                printf ("\n Value of c[%d][%d]:%f\n",i,j,Cs[i*N+j]);
        }
    }
    

    // release resources and return
    free(A);
	free(B);
	free(Cs);
	free(Cp);
	
	return 0;
}


int initialize(double * matrix)
{
	srand((unsigned)time(NULL));
    int i, j;
	for(i=0; i<N; i++)
	  for(j=0; j<N; j++)
	  {
		  matrix[i*N+j]=rand()/3000.0;
	  }
	printf("Initialized successfully!\n");
	return 0;
}

int _initialize(double * matrix)
{
    int i,j;
	for(i=0; i<N; i++)
	  for(j=0; j<N; j++)
	  {
		  matrix[i*N+j]=0.0;
	  }
	printf("Initialized successfully!\n");
	return 0;
}

/*
 
 
 
 
 
 
 
 
 */
