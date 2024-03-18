#include "cpu.h"

typedef struct
{
    int start;
    int end;
    int width;
    int height;
} ThreadData;

typedef struct
{
    float x;
    float y;
    float z;
} float3;

float3 *pixels;

float3 ray_trace(int x, int y, int width, int height)
{
    float3 ray_origin = {0.0f, 0.0f, 0.0f};
    float3 ray_direction = {(float)x / (float)width - 0.5f, (float)y / (float)height - 0.5f, 1.0f};

    float3 sphere_center = {0.0f, 0.0f, 5.0f};
    float sphere_radius = 1.0f;
    float3 oc = {ray_origin.x - sphere_center.x, ray_origin.y - sphere_center.y, ray_origin.z - sphere_center.z};
    float a = ray_direction.x * ray_direction.x + ray_direction.y * ray_direction.y + ray_direction.z * ray_direction.z;
    float b = 2.0f * (oc.x * ray_direction.x + oc.y * ray_direction.y + oc.z * ray_direction.z);
    float c = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z - sphere_radius * sphere_radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        return (float3){0.0f, 0.0f, 0.0f};
    }
    else
    {
        return (float3){1.0f, 1.0f, 1.0f};
    }
}

void *ray_tracing(void *arg)
{
    // Get the thread data
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start; i < data->end; i++)
    {
        // Get the x and y coordinates
        int x = i % data->width;
        int y = i / data->width;

        pixels[i] = ray_trace(x, y, data->width, data->height);
    }
    return NULL;
}

void cpu_ray_tracing(int width, int height, int num_threads)
{

    // Allocate memory for the pixels
    pixels = (float3 *)malloc(width * height * sizeof(float3));

    // Create threads
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    clock_t start = clock();
    // Start threads
    for (int i = 0; i < num_threads; i++)
    {
        // Set the start and end of the thread
        thread_data[i].start = i * (width * height / num_threads);
        thread_data[i].end = (i + 1) * (width * height / num_threads);
        thread_data[i].width = width;
        thread_data[i].height = height;
        pthread_create(&threads[i], NULL, ray_tracing, &thread_data[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();
    double runtime = ((double)(end - start));

    printf("CPU Runtime: %.0f ms\n", runtime);
    // Free memory
    free(pixels);
}