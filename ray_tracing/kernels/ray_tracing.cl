__kernel void ray_tracing(__global float3 *pixel_buffer, int width,
                          int height) {
  int x = get_global_id(0);
  int y = get_global_id(1);

  if (x >= width || y >= height)
    return;

  float3 ray_origin = (float3)(0.0f, 0.0f, 0.0f);
  float3 ray_direction = (float3)((float)x / (float)width - 0.5f,
                                  (float)y / (float)height - 0.5f, 1.0f);

  float3 sphere_center = (float3)(0.0f, 0.0f, 5.0f);
  float sphere_radius = 1.0f;
  // Ray-sphere intersection
  float3 oc = ray_origin - sphere_center;
  float a = dot(ray_direction, ray_direction);
  float b = 2.0f * dot(oc, ray_direction);
  float c = dot(oc, oc) - sphere_radius * sphere_radius;
  float discriminant = b * b - 4 * a * c;

  if (discriminant < 0) {
    pixel_buffer[y * width + x] = (float3)(0.0f, 0.0f, 0.0f);
  } else {
    pixel_buffer[y * width + x] = (float3)(1.0f, 1.0f, 1.0f);
  }
}