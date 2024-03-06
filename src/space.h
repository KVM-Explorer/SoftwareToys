#include "geometry.h"

// 使用左手系

using Matrix = mat<4, 4>;

Matrix lookAt(vec3 eye, vec3 center, vec3 up);

Matrix viewport(int x,int y,int width, int height);

// near far 应该是有符号负数
Matrix project(float near, float far);
Matrix projectNDC(float near, float far) ;