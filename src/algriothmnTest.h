#include "tgaimage.h"
#include "graphics.h"

const TGAColor RED =  TGAColor(255, 0, 0,0);
const TGAColor GREEN = TGAColor(0, 255, 0, 0);
const TGAColor BLUE = TGAColor(0, 0, 255, 0);
const TGAColor WHITE = TGAColor(255, 255, 255, 0);
TGAImage LineModelTest()
{
    TGAImage image(800,800,TGAImage::RGB);
    Model model("obj/african_head.obj");
    drawLineModel(model, image, WHITE);
    return image;
}

TGAImage TriangleBoundaryTest()
{
    TGAImage image(200, 200, TGAImage::RGB);
    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)}; 
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)}; 
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)}; 
    triangleLine(t0[0], t0[1], t0[2], image, RED); 
    triangleLine(t1[0], t1[1], t1[2], image, WHITE); 
    triangleLine(t2[0], t2[1], t2[2], image, GREEN);
    return image;
}

TGAImage scanTriangleTest()
{
    TGAImage image(200, 200, TGAImage::RGB);
    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)}; 
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)}; 
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
    scanTriangle(t0[0], t0[1], t0[2], image, RED); 
    scanTriangle(t1[0], t1[1], t1[2], image, WHITE); 
    scanTriangle(t2[0], t2[1], t2[2], image, GREEN);
    return image;
}

TGAImage paralleRasterationTest()
{
    TGAImage image(200, 200, TGAImage::RGB); 
    std::array<Vec2i,3> pts = {Vec2i(10,10), Vec2i(100, 30), Vec2i(190, 160)}; 
    boundingboxTriange(pts, image, RED);
    return image;
}

TGAImage flatShadingTest()
{
    TGAImage image(800, 800, TGAImage::RGB);
    Model model("obj/african_head.obj");
    flatShadingModel(model, image);
    return image;
}