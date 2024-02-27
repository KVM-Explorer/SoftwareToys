#include <iostream>
#include "tgaimage.h"
#include "geometry.h"

using namespace std;

const TGAColor RED =  TGAColor(255, 0, 0,0);
const TGAColor GREEN = TGAColor(0, 255, 0, 0);
const TGAColor BLUE = TGAColor(0, 0, 255, 0);
const TGAColor WHITE = TGAColor(255, 255, 255, 0);

int main(int argc, char** argv)
{
    TGAImage image(100,100,TGAImage::RGB);

    lineV1(0, 0, 40, 40, image,WHITE);
    lineV2(0, 20, 40, 60, image,RED);
    lineV2(13, 20, 80, 40, image, WHITE); 
    lineV2(20, 13, 40, 80, image, RED); 
    lineV2(80, 40, 13, 20, image, RED);

    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}
