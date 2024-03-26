__kernel void ray_tracing(__global float3* pixel_buffer, int width, int height)
{
    int x = get_global_id(0);
    int y = get_global_id(1);

    if (x >= width || y >= height)
        return;

    float3 ray_origin = (float3)(0.0f, 0.0f, 0.0f);
    float3 ray_direction = (float3)(
        (float)x / (float)width - 0.5f,
        (float)y / (float)height - 0.5f,
        1.0f
    );

    float3 sphere_center = (float3)(0.0f, 0.0f, 5.0f);
    float sphere_radius = 1.0f;

    float3 oc = ray_origin - sphere_center;
    float a = dot(ray_direction, ray_direction);
    float b = 2.0f * dot(oc, ray_direction);
    float c = dot(oc, oc) - sphere_radius * sphere_radius;
    float discriminant = b * b - 4 * a * c;

   if (discriminant < 0)
    {
        pixel_buffer[y * width + x] = (float3){0.0f, 0.0f, 0.0f}; // black color for no intersection
    }
    else
    {
        // calculate the inter  section point
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

        pixel_buffer[y * width + x] = (float3){diffuse, diffuse, diffuse};
    }
}