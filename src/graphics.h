#include "model.h"
#include "tgaimage.h"
#include <array>

void lineV1(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void lineV2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void lineV3(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

void drawLineModel(Model &model, TGAImage &image, TGAColor color);

void triangleLine(vec2 t0, vec2 t1, vec2 t2, TGAImage &image,
                  TGAColor color);

void scanTriangle(vec2 t0, vec2 t1, vec2 t2, TGAImage &image,
                  TGAColor color);

vec3 barycentric(std::array<vec2, 3> verts, vec2 P);
vec3 barycentric(std::array<vec3, 3> verts, vec3 P);

void boundingboxTriange(std::array<vec2, 3> verts, TGAImage &image,
                        TGAColor color);

void rasterize(std::array<vec3, 3> verts, std::vector<double> &zbuffer,
               TGAImage &image, TGAColor color);

vec3 world2screenCoords(vec3 v,vec2 screen_size);