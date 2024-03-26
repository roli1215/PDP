#ifndef CPU_H
#define CPU_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "float3.h"
#include <math.h>

typedef struct
{
    int start;
    int end;
    int width;
    int height;
} ThreadData;


float3* cpu_ray_tracing(int width, int height, int num_threads, float3* pixels);

#endif // CPU_H