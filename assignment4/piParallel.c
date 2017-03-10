//
//  main.c
//  assignment4
//
//  Created by Trent Callan on 3/10/17.
//  Copyright © 2017 Trent Callan. All rights reserved.
//

#include <stdio.h>
#include "omp.h"

static long num_steps = 100000;
double step;
long stepsCompleted = 0;
double ids[10];
int numThreads = 7;

void main (){
    int i;
    double x,pi,sum = 0.0;
    step = 1.0/(double) num_steps;
    for (i=0;i< num_steps; i++){
        x = (i+0.5)*step;
        sum = sum + 4.0/(1.0+x*x);
    }
    pi = step * sum;
    printf("sequential done with pi = %f\n", pi);
    
    x = 0;
    pi = 0;
    sum = 0;
    
    
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
            
            printf("start[%d] = %d", i, start[i]);
            printf("end[%d] = %d", i, end[i]);
            
            nextStartPoint = endPoint + 1;
        }
    }
    else{
        for (i = 0; i < numThreads; i++) {
            start[i] = (numOfStepsPerThread * i);
            end[i] = (numOfStepsPerThread * (i+1));
            
        }
    }
    
    
    
#pragma omp parallel
    {
        int mySum = 0;
        long i;
        int myId = omp_get_thread_num();
        printf("my id is %d with %lu steps of work\n", myId, numOfStepsPerThread);
        // do calc
        for (i = (numOfStepsPerThread * myId); i < (numOfStepsPerThread * (myId + 1)); i++) {
            x = (i+0.5)*step;
            mySum = mySum + 4.0/(1.0+x*x);
        }
        
        sum += mySum;
    }
    pi = step * sum;

printf("parallel end with pi = %f\n", pi);
}

