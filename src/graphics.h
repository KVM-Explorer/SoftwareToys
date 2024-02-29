#include "tgaimage.h"
#include "model.h"
void lineV1(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void lineV2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void lineV3(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

void drawLineModel(Model &model, TGAImage &image, TGAColor color);
