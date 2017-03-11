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

