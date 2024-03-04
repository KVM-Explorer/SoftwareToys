#include "pipeline.h"
#include "graphics.h"
void pipeline(std::array<vec4, 3> verts, IShader &shader, TGAImage &image,
              TGAImage &zbuffer) {

  vec2 bboxmin(static_cast<float>(image.width() - 1),
               static_cast<float>(image.height() - 1));
  vec2 bboxmax(0, 0);

  vec2 clamp(static_cast<float>(image.width() - 1),
             static_cast<float>(image.height() - 1));
  for (int i = 0; i < 3; i++) {
    using std::max;
    using std::min;
    for (int j = 0; j < 2; j++) {
      bboxmin[j] =
          max(0.0, min(bboxmin.y, verts[i][j] / verts[i][3])); // 齐次项归一
      bboxmax[j] =
          min(clamp.x, max(bboxmax.x, verts[i][j] / verts[i][3])); // 齐次项归一
    }
  }

  for (int i = bboxmin.x; i <= bboxmax.x; i++) {
    for (int j = bboxmin.y; j <= bboxmax.y; j++) {
      // Vector X
      // barycentric

      vec3 p = barycentric({proj<2>(verts[0] / verts[0][3]),
                            proj<2>(verts[1] / verts[1][3]),
                            proj<2>(verts[2] / verts[2][3])},
                           {static_cast<double>(i), static_cast<double>(j)});
      if (p.x < 0 || p.y < 0 || p.z < 0)
        continue;
      float z = p.x * verts[0][2] + p.y * verts[1][2] + p.z * verts[2][2];
      float w = p.x * verts[0][3] + p.y * verts[1][3] + p.z * verts[2][3];

      int frag_depth =  std::max(0, std::min(255, int(z / w + 0.5)));
      if (zbuffer.get(i, j)[0] < frag_depth) {
        zbuffer.set(i, j, {static_cast<uint8_t>(frag_depth), 0, 0, 0});
        TGAColor ret;
        shader.fragment(p, ret);
        image.set(i,j,ret);
      }
    }
  }
}