#include "gpu.h"
#include "cpu.h"
#include "filewriter.h"

const int width = 1920;
const int height = 1080;

int main(void)
{
    float3 *pixels = (float3 *)malloc(width * height * sizeof(float3));
    float3 *cpu_res = cpu_ray_tracing(width, height, 2, pixels);

    write_cpu_result_to_file(width, height, cpu_res);
    free(pixels);
    gpu_ray_tracing(width, height);
    return 0;
}
