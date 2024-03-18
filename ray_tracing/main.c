#include "gpu.h"
#include "cpu.h"

const int width = 1920;
const int height = 1080;

int main(void)
{
    cpu_ray_tracing(width, height, 2);
    gpu_ray_tracing(width, height);
    return 0;
}
