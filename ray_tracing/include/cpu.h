#ifndef CPU_H
#define CPU_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void cpu_ray_tracing(int width, int height, int num_threads);

#endif