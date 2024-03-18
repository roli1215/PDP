#include "gpu.h"

void gpu_ray_tracing(int width, int height)
{
    int i;
    cl_int err;
    int error_code;

    // Get platform
    cl_uint n_platforms;
    cl_platform_id platform_id;
    err = clGetPlatformIDs(1, &platform_id, &n_platforms);
    if (err != CL_SUCCESS)
    {
        printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", err);
    }

    // Get device
    cl_device_id device_id;
    cl_uint n_devices;
    err = clGetDeviceIDs(
        platform_id,
        CL_DEVICE_TYPE_GPU,
        2,
        &device_id,
        &n_devices);
    if (err != CL_SUCCESS)
    {
        printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", err);
    }

    // Create OpenCL context
    cl_context context = clCreateContext(NULL, n_devices, &device_id, NULL, NULL, NULL);

    // Build the program
    const char *kernel_code = load_kernel_source("kernels/ray_tracing.cl", &error_code);
    if (error_code != 0)
    {
        printf("Source code loading error!\n");
    }
    cl_program program = clCreateProgramWithSource(context, 1, &kernel_code, NULL, NULL);
    const char options[] = "";
    err = clBuildProgram(
        program,
        1,
        &device_id,
        options,
        NULL,
        NULL);
    if (err != CL_SUCCESS)
    {
        printf("Build error! Code: %d\n", err);
        size_t real_size;
        err = clGetProgramBuildInfo(
            program,
            device_id,
            CL_PROGRAM_BUILD_LOG,
            0,
            NULL,
            &real_size);
        char *build_log = (char *)malloc(sizeof(char) * (real_size + 1));
        err = clGetProgramBuildInfo(
            program,
            device_id,
            CL_PROGRAM_BUILD_LOG,
            real_size + 1,
            build_log,
            &real_size);
        // build_log[real_size] = 0;
        printf("Real size : %d\n", real_size);
        printf("Build log : %s\n", build_log);
        free(build_log);
    }
    cl_kernel kernel = clCreateKernel(program, "ray_tracing", NULL);
    cl_mem pixel_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, width * height * sizeof(cl_float3), NULL, &err);

    // Set kernel arguments
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &pixel_buffer);
    err = clSetKernelArg(kernel, 1, sizeof(int), &width);
    err = clSetKernelArg(kernel, 2, sizeof(int), &height);

    // Create the command queue
    cl_command_queue command_queue = clCreateCommandQueue(
        context, device_id, CL_QUEUE_PROFILING_ENABLE, NULL);

    // Apply the kernel on the range
    cl_event event;
    size_t global_size[2] = {width, height};
    err = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_size, NULL, 0, NULL, &event);
    clFinish(command_queue);

    cl_float3 *pixels = (cl_float3 *)malloc(width * height * sizeof(cl_float3));
    clEnqueueReadBuffer(command_queue, pixel_buffer, CL_TRUE, 0, width * height * sizeof(cl_float3), pixels, 0, NULL, NULL);

    // Show profiling information
    cl_ulong startNs;
    cl_ulong endNs;
    err = clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_QUEUED,
        sizeof(startNs),
        &startNs,
        NULL);
    if (err == CL_PROFILING_INFO_NOT_AVAILABLE)
    {
        printf("Profiling info not available!\n");
    }
    else if (err != CL_SUCCESS)
    {
        printf("Error code: %d\n", err);
    }
    clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_END,
        sizeof(endNs),
        &endNs,
        NULL);
    printf("GPU Runtime: %lu ms\n", (endNs - startNs) / 1000000);

    // Release the resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(device_id);

    clReleaseMemObject(pixel_buffer);
    clReleaseCommandQueue(command_queue);
    free(pixels);
}