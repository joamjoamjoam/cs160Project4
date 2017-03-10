//
//  main.c
//  assignment4
//
//  Created by Trent Callan on 3/10/17.
//  Copyright © 2017 Trent Callan. All rights reserved.
//

#include <stdio.h>
#include "omp.h"

int main(int argc, const char * argv[]) {
    #pragma parallel
    {
        int id = 0;
        
        printf("hello(%d)", id);
        printf("world(%d)", id);
        
    }
    return 0;
}
