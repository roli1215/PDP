#include "filewriter.h"

void write_cpu_result_to_file(int width, int height, float3 *result)
{
    FILE *file;

    file = fopen("output.ppm", "w");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    fprintf(file, "P3\n%d %d\n255\n", width, height);

    for (int i = 0; i < width * height; i++)
    {
        fprintf(file, "%d %d %d\n", (int)(result[i].x * 255.99), (int)(result[i].y * 255.99), (int)(result[i].z * 255.99));
    }

    fclose(file);

    printf("Data written to file successfully.\n");
}