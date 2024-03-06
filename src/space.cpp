#include "space.h"

Matrix lookAt(vec3 eye, vec3 center, vec3 up) {

  vec3 z = (eye - center).normalized();
  vec3 x = cross(up, z).normalized();
  vec3 y = cross(z, x).normalized();

  // 由逆分解tranform 和 rotation
  Matrix invR = Matrix::identity();
  Matrix transform = Matrix::identity();

  for (int i = 0; i < 3; i++) {
    invR[0][i] = x[i];             // R可以认为由x,y,z向量竖排排列而成，这里直接横排相当于转置或者说inR
    invR[1][i] = y[i];
    invR[2][i] = z[i];
    transform[i][3] = -eye[i];
  }
  return invR * transform; //  因为 corrds : origin ->R-> view  | inR * worldObject = viewObject 
}

Matrix viewport(int width, int height,int near,int far) {
  Matrix ret = Matrix::identity();
  ret[0][0] = width / 2; // x/2 * width
  ret[0][3] = width / 2; // x + 1/2 * width

  ret[1][1] = height / 2;
  ret[1][3] = height / 2;

  // ret[2][2] = -1 / (near - far);
  // ret[2][3] = near / (near-far);

  ret[2][2] = -255 / 2; // 翻转为-z，使得0-255 本来越小越近，翻转成越大越近
  ret[2][3] = 255 / 2;

  // ignore depth Process
  return ret;
}

Matrix project(float near, float far) {
  Matrix ret = Matrix::identity();
  ret[3][3] = 0;

  ret[0][0] = ret[1][1] = near;
  ret[2][2] = (near + far) / (near - far);
  ret[2][3] = -( near * far) / (near - far);
  ret[3][2] = 1;
  return ret;
}

