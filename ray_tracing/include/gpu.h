#ifndef GPU_H
#define GPU_H

#include "kernel_loader.h"

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <CL/cl_platform.h>

#include <stdio.h>
#include <stdlib.h>

void gpu_ray_tracing(int width, int height);

#endif