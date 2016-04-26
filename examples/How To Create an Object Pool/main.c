//
//  main.c
//  How To Create an Object Pool
//
//  Created by gscott on 4/22/16.
//  Copyright © 2016 CSL. All rights reserved.
//

#include <stdio.h>

#include "parc_BufferPool.h"

int
main(int argc, char *argv[argc])
{
    PARCBufferPool *pool = parcBufferPool_Create(10);
    
    PARCBuffer *buffer = parcBufferPool_GetInstance(pool);
    parcBuffer_Release(&buffer);

    buffer = parcBufferPool_GetInstance(pool);
    parcBuffer_Release(&buffer);
    
    parcBufferPool_Release(&pool);
    
    return 0;
}