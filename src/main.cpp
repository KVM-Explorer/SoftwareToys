#include <iostream>
#include "tgaimage.h"
#include "graphics.h"

using namespace std;

const TGAColor RED =  TGAColor(255, 0, 0,0);
const TGAColor GREEN = TGAColor(0, 255, 0, 0);
const TGAColor BLUE = TGAColor(0, 0, 255, 0);
const TGAColor WHITE = TGAColor(255, 255, 255, 0);

int main(int argc, char** argv)
{
    

    TGAImage image(800,800,TGAImage::RGB);
    Model model("obj/african_head.obj");

    // lineV1(0, 0, 40, 40, image,WHITE);
    // lineV2(0, 20, 40, 60, image,RED);
    // lineV2(13, 20, 80, 40, image, WHITE); 
    // lineV2(20, 13, 40, 80, image, RED); 
    // lineV2(80, 40, 13, 20, image, RED);

    drawLineModel(model, image, WHITE);
    // lineV3(400,400,400,800,image,RED);
    // lineV3(400,400,800,400,image,RED);


    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}
