#include <iostream>
#include "tgaimage.h"
using namespace std;

const TGAColor red =  TGAColor(255, 0, 0,0);
const TGAColor green = TGAColor(0, 255, 0, 0);
const TGAColor blue = TGAColor(0, 0, 255, 0);
const TGAColor white = TGAColor(255, 255, 255, 0);

int main(int argc, char** argv)
{
    TGAImage image(100,100,TGAImage::RGB);

    image.set(10, 50, red);
    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}
