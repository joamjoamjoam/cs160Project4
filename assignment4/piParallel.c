//
//  main.c
//  assignment4
//
//  Created by Trent Callan on 3/10/17.
//  Copyright © 2017 Trent Callan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

static long num_steps = 100000;
double step;
long stepsCompleted = 0;
double ids[10];
int numThreads = 7;

void main (int argc, char** argv){
    double begin = omp_get_wtime();
    int i;
    double x,pi,sum = 0.0;
    numThreads = atoi(argv[1]);
    step = 1.0/(double) num_steps;
    for (i=0;i< num_steps; i++){
        x = (i+0.5)*step;

        sum = sum + 4.0/(1.0+x*x);
    }
    pi = step * sum;
    double endTime = omp_get_wtime();
    printf("sequential done with %d threads, pi = %f in %f seconds\n", numThreads, pi, endTime - begin);
    
    pi = 0.0;
    sum = 0.0;
    
    begin = omp_get_wtime();
    
    omp_set_num_threads(numThreads);
    int start[numThreads];
    int end[numThreads];
    double numOfStepsPerThread = (double) num_steps/ (double)numThreads;
    
    if ((int) numOfStepsPerThread != numOfStepsPerThread) {
        // is a floating point
        int nextStartPoint = 0;
        for (i = 0; i < numThreads; i++) {
            int endPoint = (int) (numOfStepsPerThread * (i + 1));
            
            start[i] = nextStartPoint;
            end[i] = endPoint;
            
            nextStartPoint = endPoint;
        }
    }
    else{
        for (i = 0; i < numThreads; i++) {
            start[i] = (numOfStepsPerThread * i);
            end[i] = (numOfStepsPerThread * (i+1));
            
        }
    }
    double mySums[numThreads];
#pragma omp parallel
    {
        double mySum = 0;
        int i = 0;
        double myX = 0;
        int myId = omp_get_thread_num();
        //printf("my id is %d with %d steps of work\n", myId, end[myId] - start[myId]);
        // do calc
        for (i = start[myId]; i < end[myId]; i++) {
            myX = (i+0.5)*step;
            
            //            if (myId == 1 && i >= start[myId] && i <= start[myId] + 10) {
            //                printf("Thread x = %f for step %d\n", myX, i);
            //            }
            
            mySum = mySum + 4.0/(1.0+myX*myX);
        }
        
        mySums[myId] = mySum;
    }
    for (i = 0; i < numThreads; i++) {
        sum += mySums[i];
    }
    
    pi = step * sum;
    
    endTime = omp_get_wtime();
    printf("parallel end with %d threads, pi = %f in %f seconds\n", numThreads ,pi, endTime - begin);
    
    // start looped
    
    pi = 0;
    sum = 0;
    
    begin = omp_get_wtime();
# pragma omp parallel
    {
        double loopX = 0;
        double loopSum = 0;
        step = 1.0/(double) num_steps;
        # pragma omp for reduction (+:sum)
            for (i=0; i < num_steps; i++){
                loopX = (i+0.5)*step;
                sum = sum + 4.0/(1.0+loopX*loopX);
            }
    }
    pi = step * sum;
    
    
    endTime = omp_get_wtime();
    
    printf("Looped end with %d threads, pi = %f in %f seconds\n", numThreads, pi, endTime - begin);
    
}

/***************************************************************************************************
                                            Timings
 1 Thread:
 
 sequential done with 1 threads, pi = 3.141593 in 0.002135 seconds
 parallel end with 1 threads, pi = 3.141593 in 0.002113 seconds
 Looped end with 1 threads, pi = 3.141593 in 0.002095 seconds
 
 2 Threads:
 
 sequential done with 2 threads, pi = 3.141593 in 0.002094 seconds
 parallel end with 2 threads, pi = 3.141593 in 0.001110 seconds
 Looped end with 2 threads, pi = 3.141593 in 0.001062 seconds
 
 3 Threads:
 
 sequential done with 3 threads, pi = 3.141593 in 0.002095 seconds
 parallel end with 3 threads, pi = 3.141593 in 0.000774 seconds
 Looped end with 3 threads, pi = 3.141593 in 0.000698 seconds
 
 4 Threads:
 
 sequential done with 4 threads, pi = 3.141593 in 0.002095 seconds
 parallel end with 4 threads, pi = 3.141593 in 0.000617 seconds
 Looped end with 4 threads, pi = 3.141593 in 0.000524 seconds
 
 5 Threads:
 
 sequential done with 5 threads, pi = 3.141593 in 0.002095 seconds
 parallel end with 5 threads, pi = 3.141593 in 0.000524 seconds
 Looped end with 5 threads, pi = 3.141593 in 0.000445 seconds
 
 6 Threads:
 
 sequential done with 6 threads, pi = 3.141593 in 0.002302 seconds
 parallel end with 6 threads, pi = 3.141593 in 0.000563 seconds
 Looped end with 6 threads, pi = 3.141593 in 0.000427 seconds
 
 7 Threads:
 
 sequential done with 7 threads, pi = 3.141593 in 0.002101 seconds
 parallel end with 7 threads, pi = 3.141593 in 0.000467 seconds
 Looped end with 7 threads, pi = 3.141593 in 0.000304 seconds
 
 8 Threads:
 
 sequential done with 8 threads, pi = 3.141593 in 0.002092 seconds
 parallel end with 8 threads, pi = 3.141593 in 0.000374 seconds
 Looped end with 8 threads, pi = 3.141593 in 0.000265 seconds
 
 */




