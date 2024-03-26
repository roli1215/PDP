#include "cpu.h"

float3 *pixels;

float3 ray_trace(int x, int y, int width, int height)
{
    float3 ray_origin = {0.0f, 0.0f, 0.0f};
    float3 ray_direction = {(float)x / (float)width - 0.5f, (float)y / (float)height - 0.5f, 1.0f};

    float3 sphere_center = {0.0f, 0.0f, 1.0f};
    float sphere_radius = 0.5f;

    float3 oc = {ray_origin.x - sphere_center.x, ray_origin.y - sphere_center.y, ray_origin.z - sphere_center.z};
    float a = ray_direction.x * ray_direction.x + ray_direction.y * ray_direction.y + ray_direction.z * ray_direction.z;
    float b = 2.0f * (oc.x * ray_direction.x + oc.y * ray_direction.y + oc.z * ray_direction.z);
    float c = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z - sphere_radius * sphere_radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        return (float3){0.0f, 0.0f, 0.0f}; // black color for no intersection
    }
    else
    {
        // calculate the intersection point
        float t = (-b - sqrt(discriminant)) / (2.0f * a);
        float3 point = {ray_origin.x + t * ray_direction.x, ray_origin.y + t * ray_direction.y, ray_origin.z + t * ray_direction.z};

        // calculate the normal at the intersection point
        float3 normal = {point.x - sphere_center.x, point.y - sphere_center.y, point.z - sphere_center.z};
        float normal_length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        normal.x /= normal_length;
        normal.y /= normal_length;
        normal.z /= normal_length;

        // calculate the light direction
        float3 light_position = {0.0f, 0.0f, 0.0f};
        float3 light_direction = {light_position.x - point.x, light_position.y - point.y, light_position.z - point.z};
        float light_direction_length = sqrt(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
        light_direction.x /= light_direction_length;
        light_direction.y /= light_direction_length;
        light_direction.z /= light_direction_length;

        // calculate the diffuse lighting
        float diffuse = light_direction.x * normal.x + light_direction.y * normal.y + light_direction.z * normal.z;
        if (diffuse < 0)
            diffuse = 0;

        return (float3){diffuse, diffuse, diffuse}; // white color for intersection, modulated by diffuse lighting
    }
}

void *ray_tracing(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start; i < data->end; i++)
    {
        int x = i % data->width;
        int y = i / data->width;
        pixels[i] = ray_trace(x, y, data->width, data->height);
    }
    return NULL;
}

float clamp(float val, float min, float max)
{
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
}

float3 *cpu_ray_tracing(int width, int height, int num_threads, float3 *pixels)
{

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    clock_t start = clock();

    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].start = i * (width * height / num_threads);
        thread_data[i].end = (i + 1) * (width * height / num_threads);
        thread_data[i].width = width;
        thread_data[i].height = height;
        pthread_create(&threads[i], NULL, ray_tracing, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();
    double runtime = ((double)(end - start));

    printf("CPU Runtime: %f ms\n", runtime);

    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            pixels[i].x = (clamp(pixels[i].x / width, 0.0f, 1.0f));
            pixels[i].y = (clamp(pixels[j].y / height, 0.0f, 1.0f));
            pixels[i].z = (clamp(pixels[i].z, 0.0f, 1.0f));
            printf("%d %d %d\n", (int)pixels[i].x, (int)pixels[i].y, (int)pixels[i].z);
        }
    }


    return pixels;
}