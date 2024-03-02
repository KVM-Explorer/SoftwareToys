#include "graphics.h"
#include <format>
#include <iostream>

void lineV1(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
  float delta = 0;
  while (delta <= 1.0F) {
    float x = x0 + delta * (x1 - x0);
    float y = y0 + delta * (y1 - y0);
    image.set(x, y, color);
    delta += 0.01;
  }
}

void lineV2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
  bool is_reverse = false;
  if (abs(y1 - y0) >
      abs(x1 -
          x0)) { // 确定按照XY次序步进，若k<1则按照x步进，反之按照y步进，通过Swap后下文可以均按照x步进
    std::swap(x0, y0);
    std::swap(x1, y1);
    is_reverse = true;
  }

  if (x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  int dx = x1 - x0;
  int dy = y1 - y0;
  float eps = float(dy) / float(dx);

  float eps_sum = 0;
  while (x0 < x1) {
    if (is_reverse) {
      image.set(y0, x0, color);
    } else {
      image.set(x0, y0, color);
    }
    eps_sum += eps;
    if (std::abs(eps_sum) > 0.5) {
      y0 += (y1 > y0) ? 1 : -1;
      eps_sum += eps_sum > 0.5 ? -1 : 1;
    }
    x0 = x0 + 1;
  }
}

void lineV3(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
  bool is_reverse = false;
  if (abs(y1 - y0) >
      abs(x1 -
          x0)) { // 确定按照XY次序步进，若k<1则按照x步进，反之按照y步进，通过Swap后下文可以均按照x步进
    std::swap(x0, y0);
    std::swap(x1, y1);
    is_reverse = true;
  }

  if (x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  int dx = x1 - x0;
  int dy = y1 - y0;
  int eps = abs(
      dy *
      2); // 替代之前求斜率的想法，取而代之的是直接和分母对应的整数比较，注意后续比较范围为-0.5
          // ~ 0.5 可以取斜率绝对值简化逻辑，

  int eps_sum = 0;
  int y = y0;
  while (x0 < x1) {
    if (is_reverse) {
      image.set(y, x0, color);
    } else {
      image.set(x0, y, color);
    }
    eps_sum += eps;
    if (eps_sum >
        dx) { // 因为其只关心增长速度不关心增长方向，为避免小数使用*2转化为对应的原始分母
      y += (y1 > y0) ? 1 : -1;
      eps_sum -= 2 * dx; // 注意是两倍的dx
    }
    x0 = x0 + 1;
  }
}

void drawLineModel(Model &model, TGAImage &image, TGAColor color) {
  int width = image.width();
  int height = image.height();
  for (int i = 0; i < model.nfaces(); i++) {
    for (int j = 0; j < 3; j++) {
      vec3 v0 = model.vert(i, j);
      vec3 v1 = model.vert(i, (j + 1) % 3);

      int x0 = (v0.x + 1.0F) * width / 2.0F;
      int y0 = (v0.y + 1.0F) * height / 2.0F;
      int x1 = (v1.x + 1.0F) * width / 2.0F;
      int y1 = (v1.y + 1.0F) * height / 2.0F;
      lineV3(x0, y0, x1, y1, image, color);
    }
  }
}

void triangleLine(vec2 t0, vec2 t1, vec2 t2, TGAImage &image, TGAColor color) {
  lineV3(t0.x, t0.y, t1.x, t1.y, image, color);
  lineV3(t1.x, t1.y, t2.x, t2.y, image, color);
  lineV3(t2.x, t2.y, t0.x, t0.y, image, color);
}

void scanTriangle(vec2 t0, vec2 t1, vec2 t2, TGAImage &image, TGAColor color) {
  if (t0.y == t1.y && t0.y == t2.y)
    return;
  // t0 < t1 < t2
  if (t0.y > t1.y)
    std::swap(t0, t1);
  if (t1.y > t2.y)
    std::swap(t1, t2);
  if (t0.y > t1.y)
    std::swap(t0, t1);

  int height = t2.y - t0.y;
  int half_height = t1.y - t0.y;
  for (int i = 0; i <= height; i++) {
    float alpha = float(i) / float(t2.y - t0.y); // t0 - t2
    int left = alpha * float(t2.x - t0.x) + t0.x;

    if (left < 0) {
      int x = 1;
    }
    if (i <= half_height && t1.y != t0.y) {
      float beta = float(i) / float(t1.y - t0.y); // t0 - t1

      int right = beta * float(t1.x - t0.x) + t0.x;

      if (left > right)
        std::swap(right, left);

      for (int j = left; j <= right; j++) {
        image.set(j, i + t0.y, color);
      }
    } else {
      float beta = float(i - half_height) / float(t2.y - t1.y);
      int right = beta * float(t2.x - t1.x) + t1.x;
      if (left > right)
        std::swap(left, right);
      for (int j = left; j <= right; j++) {
        image.set(j, i + t0.y, color);
      }
    }
  }
}

// TODO: Fix BoundingBoxTriangle Bug
vec3 barycentric(std::array<vec2, 3> verts, vec2 P) {
  // 向量叉乘判定点在内外侧
  auto v1 =
      vec3(verts[2].x - verts[0].x, verts[1].x - verts[0].x, verts[0].x - P.x);
  auto v2 =
      vec3(verts[2].y - verts[0].y, verts[1].y - verts[0].y, verts[0].y - P.y);
  auto u = cross(v1, v2);
  if (std::abs(u.z) < 1)
    return vec3(-1, 1, 1);
  return vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

vec3 barycentric(std::array<vec3, 3> verts, vec3 P) {
  vec3 s[2];
  for (int i = 2; i--;) { // 仅计算xy
    s[i][0] = verts[2][i] - verts[0][i];
    s[i][1] = verts[1][i] - verts[0][i];
    s[i][2] = verts[0][i] - P[i];
  }
  vec3 u = cross(s[0], s[1]);
  if (std::abs(u[2]) > 1e-2) // dont forget that u[2] is integer. If it is zero
                             // then triangle ABC is degenerate
    return vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
  return vec3(-1, 1, 1); // in this case generate negative coordinates, it will
                         // be thrown away by the rasterizator
}

void boundingboxTriange(std::array<vec2, 3> verts, TGAImage &image,
                        TGAColor color) {
  if (verts[0].y == verts[1].y && verts[1].y == verts[2].y)
    return;
  vec2 bboxmin(image.width() - 1, image.height() - 1);
  vec2 bboxmax(0, 0);

  vec2 clamp(image.width() - 1, image.height() - 1);
  for (int i = 0; i < 3; i++) {
    bboxmin.x = std::min(bboxmin.x, verts[i].x);
    bboxmin.y = std::min(bboxmin.y, verts[i].y);

    bboxmax.x = std::max(bboxmax.x, verts[i].x);
    bboxmax.y = std::max(bboxmax.y, verts[i].y);

    // 防止溢出
    bboxmin.x = std::max(0.0, bboxmin.x);
    bboxmin.y = std::max(0.0, bboxmin.y);

    bboxmax.x = std::min(clamp.x, bboxmax.x);
    bboxmax.y = std::min(clamp.y, bboxmax.y);
  }

  for (int i = bboxmin.x; i <= bboxmax.x; i++) {
    for (int j = bboxmin.y; j <= bboxmax.y; j++) {
      // Vector X
      // barycentric

      vec3 p =
          barycentric(verts, {static_cast<double>(i), static_cast<double>(j)});
      if (p.x < 0 || p.y < 0 || p.z < 0)
        continue;
      image.set(i, j, color);
    }
  }
}

// TODO: Fix Raster Z Buffer Bug
void rasterize(std::array<vec3, 3> verts, std::vector<double> &zbuffer,std::array<vec2,3> uvs, const TGAImage &diffuse,
               TGAImage &image) {
//   if (verts[0].y == verts[1].y && verts[1].y == verts[2].y)
//     return;
  vec2 bboxmin(static_cast<float>(image.width() - 1),
               static_cast<float>(image.height() - 1));
  vec2 bboxmax(0, 0);

  vec2 clamp(static_cast<float>(image.width() - 1),
             static_cast<float>(image.height() - 1));
  for (int i = 0; i < 3; i++) {
    bboxmin.x = std::min(bboxmin.x, verts[i].x);
    bboxmin.y = std::min(bboxmin.y, verts[i].y);

    bboxmax.x = std::max(bboxmax.x, verts[i].x);
    bboxmax.y = std::max(bboxmax.y, verts[i].y);

    // 防止溢出
    bboxmin.x = std::max(0.0, bboxmin.x);
    bboxmin.y = std::max(0.0, bboxmin.y);

    bboxmax.x = std::min(clamp.x, bboxmax.x);
    bboxmax.y = std::min(clamp.y, bboxmax.y);
  }

  for (int i = bboxmin.x; i <= bboxmax.x; i++) {
    for (int j = bboxmin.y; j <= bboxmax.y; j++) {
      // Vector X
      // barycentric

      vec3 p = barycentric(verts,
                           {static_cast<double>(i), static_cast<double>(j), 0});

      if (p.x < 0 || p.y < 0 || p.z < 0)
        continue;
      double z = 0;
      vec2 uv {};
      for (int k = 0; k < 3; k++) {
        uv.x += uvs[k].x * p[k];
        uv.y += uvs[k].y * p[k];
        z += verts[k].z * p[k];  // 基于重心坐标比例对三个顶点的z插值
      }  
    
      int zbuffer_index = image.width() *j + i;

      if (zbuffer[zbuffer_index] > z) {
        auto color = diffuse.get(static_cast<int>(uv.x*diffuse.width()),static_cast<int>(uv.y*diffuse.height()));
        zbuffer[zbuffer_index] = z;
        image.set(i, j, color);
      }
    }
  }
}

vec3 world2screenCoords(vec3 v, vec2 screen_size) {
  return vec3(int((v.x + 1.0F) * screen_size.x / 2.0F),
              int((v.y + 1.0F) * screen_size.y / 2.0F), v.z);
}