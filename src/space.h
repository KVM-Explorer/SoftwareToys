#include "geometry.h"

// 使用左手系

using Matrix = mat<4, 4>;

Matrix lookAt(vec3 eye, vec3 center, vec3 up);

Matrix viewport(int width, int height);

Matrix project(float near, float far);
