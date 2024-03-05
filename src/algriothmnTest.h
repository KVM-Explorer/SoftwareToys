#include "graphics.h"
#include "pipeline.h"
#include "space.h"
#include "tgaimage.h"
#include <limits>

const TGAColor RED = TGAColor({255, 0, 0, 0});
const TGAColor GREEN = TGAColor({0, 255, 0, 0});
const TGAColor BLUE = TGAColor({0, 0, 255, 0});
const TGAColor WHITE = TGAColor({255, 255, 255, 0});
TGAImage LineModelTest() {
  TGAImage image(800, 800, TGAImage::RGB);
  Model model("obj/african_head/african_head.obj");
  drawLineModel(model, image, WHITE);
  return image;
}

TGAImage TriangleBoundaryTest() {
  TGAImage image(200, 200, TGAImage::RGB);
  vec2 t0[3] = {vec2(10, 70), vec2(50, 160), vec2(70, 80)};
  vec2 t1[3] = {vec2(180, 50), vec2(150, 1), vec2(70, 180)};
  vec2 t2[3] = {vec2(180, 150), vec2(120, 160), vec2(130, 180)};
  triangleLine(t0[0], t0[1], t0[2], image, RED);
  triangleLine(t1[0], t1[1], t1[2], image, WHITE);
  triangleLine(t2[0], t2[1], t2[2], image, GREEN);
  return image;
}

TGAImage scanTriangleTest() {
  TGAImage image(200, 200, TGAImage::RGB);
  vec2 t0[3] = {vec2(10, 70), vec2(50, 160), vec2(70, 80)};
  vec2 t1[3] = {vec2(180, 50), vec2(150, 1), vec2(70, 180)};
  vec2 t2[3] = {vec2(180, 150), vec2(120, 160), vec2(130, 180)};
  scanTriangle(t0[0], t0[1], t0[2], image, RED);
  scanTriangle(t1[0], t1[1], t1[2], image, WHITE);
  scanTriangle(t2[0], t2[1], t2[2], image, GREEN);
  return image;
}

TGAImage paralleRasterationTest() {
  TGAImage image(400, 400, TGAImage::RGB);
  std::array<vec2, 3> pts = {vec2(10, 10), vec2(100, 30), vec2(190, 160)};
  std::array<vec2, 3> t1 = {vec2{100, 0},
                            vec2{
                                100,
                                100,
                            },
                            vec2{200, 0}};
  std::array<vec2, 3> t2 = {vec2{100, 0},
                            vec2{
                                200,
                                100,
                            },
                            vec2{200, 0}};
  std::array<vec2, 3> t3 = {vec2{100, 0},
                            vec2{
                                100,
                                100,
                            },
                            vec2{200, 100}};
  std::array<vec2, 3> t4 = {vec2{100, 100},
                            vec2{
                                200,
                                100,
                            },
                            vec2{200, 0}};

  boundingboxTriange(pts, image, RED);

  boundingboxTriange(t1, image, RED);
  for (auto &&i : t2)
    i = i + vec2(100, 0);
  boundingboxTriange(t2, image, WHITE);
  for (auto &&i : t3)
    i = i + vec2(0, 100);
  boundingboxTriange(t3, image, WHITE);
  for (auto &&i : t4)
    i = i + vec2(100, 100);
  boundingboxTriange(t4, image, WHITE);

  return image;
}

TGAImage flatShadingTest() {
  TGAImage image(800, 800, TGAImage::RGB);
  Model model("obj/african_head/african_head.obj");
  int width = image.width();
  int height = image.height();
  for (int i = 0; i < model.nfaces(); i++) {
    TGAColor color = TGAColor({static_cast<uint8_t>(rand() % 255),
                               static_cast<uint8_t>(rand() % 255),
                               static_cast<uint8_t>(rand() % 255), 255});
    std::vector<vec2> verts{};
    for (int j = 0; j < 3; j++) {
      vec3 v0 = model.vert(i, j);

      int x0 = (v0.x + 1.0F) * width / 2.0F;
      int y0 = (v0.y + 1.0F) * height / 2.0F;

      verts.push_back({
          static_cast<double>(x0),
          static_cast<double>(y0),
      });
    }
    boundingboxTriange({verts[0], verts[1], verts[2]}, image, color);
    // scanTriangle(verts[0], verts[1], verts[2], image, color);
  }
  return image;
}

TGAImage simpleLightModelTest() {
  const vec3 light_dir(0, 0, -1);

  TGAImage image(800, 800, TGAImage::RGB);
  Model model("obj/african_head/african_head.obj");

  int width = image.width();
  int height = image.height();
  for (int i = 0; i < model.nfaces(); i++) {
    std::array<vec2, 3> screen_coords;
    std::array<vec3, 3> world_coords;

    for (int j = 0; j < 3; j++) {
      vec3 v0 = model.vert(i, j);

      screen_coords[j] = vec2(static_cast<int>((v0.x + 1.0F) * width / 2.0F),
                              static_cast<int>((v0.y + 1.0F) * height / 2.0F));
      world_coords[j] = v0;
    }

    // 计算世界坐标系下的法向量
    vec3 v1 = vec3(world_coords[0] - world_coords[2]);
    vec3 v2 = vec3(world_coords[0] - world_coords[1]);
    vec3 normal = cross(v1, v2);
    normal = normal.normalized();

    float intensity = normal * light_dir;
    if (intensity > 0) // 正面
      boundingboxTriange(
          screen_coords, image,
          TGAColor({static_cast<uint8_t>(intensity * 255),
                    static_cast<uint8_t>(intensity * 255),
                    static_cast<uint8_t>(intensity * 255), 255}));
  }
  return image;
}

// Zbuffer + Diffuse Map
TGAImage simpleWorldTest() {
  const vec3 light_dir(0, 0, -1);

  TGAImage image(800, 800, TGAImage::RGB);
  Model model("obj/african_head/african_head.obj");
  TGAImage diffuse_tex;

  std::vector<double> zbuffer(image.width() * image.height());

  // init zbuffer
  for (auto &&item : zbuffer)
    item = -std::numeric_limits<float>::max();

  int width = image.width();
  int height = image.height();
  for (int i = 0; i < model.nfaces(); i++) {
    std::array<vec3, 3> screen_coords;
    std::array<vec3, 3> world_coords;
    std::array<vec2, 2> diffuse_coords;
    std::array<vec2, 3> uv_coords;
    for (int j = 0; j < 3; j++) {
      vec3 v0 = model.vert(i, j);

      screen_coords[j] = world2screenCoords(
          v0, {static_cast<double>(width), static_cast<double>(height)});
      world_coords[j] = v0;
      uv_coords[j] = model.uv(i, j);
    }

    // 计算世界坐标系下的法向量
    vec3 v1 = vec3(world_coords[0] - world_coords[2]);
    vec3 v2 = vec3(world_coords[0] - world_coords[1]);
    vec3 normal = cross(v1, v2);
    normal = normal.normalized();

    // float intensity = normal * light_dir;
    // if (intensity >= 0) // 正面
    rasterize(screen_coords, zbuffer, uv_coords, model.diffuse(), image);
  }

  return image;
}

TGAImage GouraudShaderTest() {
  const vec3 eye{0, 0, 1};
  const vec3 center{0, 0, 0};
  const vec3 up(0, 1, 0);
  const vec3 lightDir{1, 1, 1};
  const int width = 800;
  const int heigth = 800;

  VSInput vsInput;
  vsInput.viewmodel = lookAt(eye, center, up);
  vsInput.viewport = viewport(800, 800);
  vsInput.project = project(1,-2);

  TGAImage image(width, heigth, TGAImage::Format::RGB);
  TGAImage zbuffer(width, heigth, TGAImage::Format::GRAYSCALE);
  Model model("obj/african_head/african_head.obj");
  GouraudShader shader(vsInput);

  // for (int i = 0; i < model.nfaces(); i++) {
  //   std::array<vec4, 3> screen_coords;
  //   for (int j = 0; j < 3; j++) {
  //     auto pt = model.vert(i, j);
  //     screen_coords[j] = shader.vertex(pt);
  //   }
  //   pipeline(screen_coords,shader,image,zbuffer);
  // }
  // Test Triangle
  auto pts = std::array<vec3, 3>{vec3{-0.5, 0, -2}, vec3{0.0, 1.0, -2.0},
                                 vec3{0.5, 0, -2.0}};

  std::array<vec4, 3> screen_coords;
  for (int j = 0; j < 3; j++) {
      screen_coords[j] = shader.vertex(pts[j]);
  }
  pipeline(screen_coords,shader,image,zbuffer);

  return image;
}
