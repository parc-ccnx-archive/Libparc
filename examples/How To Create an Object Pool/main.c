//
//  main.c
//  How To Create an Object Pool
//
//  Created by gscott on 4/22/16.
//  Copyright © 2016 CSL. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include <parc/developer/parc_Stopwatch.h>

#include "parc_BufferPool.h"


/*
 Create an array of buffers
 For N times, randomly deallocate a random set of buffers and reallocate another
 */


void
a()
{
    PARCBuffer *array[1000];
    
    PARCStopwatch *stopwatch = parcStopwatch_Create();
    parcStopwatch_Start(stopwatch);
    
    for (int i = 0; i < 1000; i++) {
        array[i] = parcBuffer_Allocate(100);
    }
    
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            size_t index = random() % 1000;
            if (array[index] != NULL) {
                parcBuffer_Release(&array[index]);
                array[index] = parcBuffer_Allocate(100);
            } else {
                j--;
            }
        }
    }
    
    for (int i = 0; i < 1000; i++) {
        parcBuffer_Release(&array[i]);
    }
    uint64_t elapsed = parcStopwatch_ElapsedTimeNanos(stopwatch);
    
    printf("%llu\n", elapsed);
}

void
b()
{
    PARCBuffer *array[1000];
    PARCSimpleBufferPool *pool = parcSimpleBufferPool_Create(100, 10);
    
    PARCStopwatch *stopwatch = parcStopwatch_Create();
    parcStopwatch_Start(stopwatch);
    
    for (int i = 0; i < 1000; i++) {
        array[i] = parcSimpleBufferPool_GetInstance(pool);
    }
    
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            size_t index = random() % 1000;
            if (array[index] != NULL) {
                parcBuffer_Release(&array[index]);
                array[index] = parcBuffer_Allocate(100);
            } else {
                j--;
            }
        }
    }
    
    for (int i = 0; i < 1000; i++) {
        parcBuffer_Release(&array[i]);
    }
    uint64_t elapsed = parcStopwatch_ElapsedTimeNanos(stopwatch);
    
    printf("%llu\n", elapsed);
    
    parcSimpleBufferPool_Release(&pool);
}

int
main(int argc, char *argv[argc])
{
    a();
    b();
    
    PARCSimpleBufferPool *pool = parcSimpleBufferPool_Create(3, 10);
    
    PARCBuffer *buffer = parcSimpleBufferPool_GetInstance(pool);
    parcBuffer_Release(&buffer);

    buffer = parcSimpleBufferPool_GetInstance(pool);
    parcBuffer_Release(&buffer);
    
    parcSimpleBufferPool_Release(&pool);
    
    return 0;
}