#include "geometry.h"

void lineV1(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    float delta = 0;
    while(delta <= 1.0F)
    {
        float x = x0 + delta * (x1 - x0);
        float y = y0 + delta * (y1 - y0);
        image.set(x, y, color);
        delta += 0.01;
    }
}

void lineV2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    if(x0>x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    float ddx = (y1 - y0) / (x1 - x0);
    bool is_reverse = false;
    if(ddx > 1){
        std::swap(x0,y0);
        std::swap(x1, y1);
    }

    float eps = 0;
    while(x0 < x1)
    {
        if(!is_reverse) 
        {
            image.set(x0,y0,color);
        }
        else{
            image.set(y0, x0, color);
        }
        x0 = x0 + 1;
        eps += ddx;
        if (std::abs(eps)>0.5){
            y1 = (y1 > y0) ? 1 : -1;
            eps += eps > 0.5 ? -1 : 1;
        }
    }
}
