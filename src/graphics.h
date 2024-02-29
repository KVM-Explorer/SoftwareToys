#include "tgaimage.h"
#include "model.h"
#include <array>
void lineV1(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void lineV2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void lineV3(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

void drawLineModel(Model &model, TGAImage &image, TGAColor color);
void flatShadingModel(Model &model, TGAImage &image);

void triangleLine(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color);

void scanTriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color);

Vec3f barycentric(std::array<Vec2i,3> verts , Vec2i P);

void boundingboxTriange(std::array<Vec2i,3> verts, TGAImage &image, TGAColor color);