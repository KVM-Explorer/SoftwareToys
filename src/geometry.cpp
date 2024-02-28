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
    bool is_reverse = false;
    if(abs(y1-y0) > abs(x1-x0)){ // 确定按照XY次序步进，若k<1则按照x步进，反之按照y步进，通过Swap后下文可以均按照x步进
        std::swap(x0,y0);
        std::swap(x1, y1);
        is_reverse = true;
    }

    if(x0>x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    float eps = float(dy) / float(dx);

    float eps_sum = 0;
    while(x0 < x1)
    {
        if(is_reverse)
        {
            image.set(y0, x0, color);
        }else{
            image.set(x0, y0, color);
        }
        eps_sum += eps;
        if (std::abs(eps_sum)>0.5){
            y0 += (y1 > y0) ? 1 : -1;
            eps_sum += eps_sum > 0.5 ? -1 : 1;
        }
        x0 = x0 + 1;
    }
}

void lineV3(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
     bool is_reverse = false;
    if(abs(y1-y0) > abs(x1-x0)){ // 确定按照XY次序步进，若k<1则按照x步进，反之按照y步进，通过Swap后下文可以均按照x步进
        std::swap(x0,y0);
        std::swap(x1, y1);
        is_reverse = true;
    }

    if(x0>x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int eps = abs(dy * 2);          // 替代之前求斜率的想法，取而代之的是直接和分母对应的整数比较，注意后续比较范围为-0.5 ~ 0.5 可以取斜率绝对值简化逻辑，

    float eps_sum = 0;
    while(x0 < x1)
    {
        if(is_reverse)
        {
            image.set(y0, x0, color);
        }else{
            image.set(x0, y0, color);
        }
        eps_sum += eps;
        if (eps_sum>dx){            // 因为其只关心增长速度不关心增长方向，为避免小数使用*2转化为对应的原始分母
            y0 += (y1 > y0) ? 1 : -1;
            eps_sum -= 2*dy;
        }
        x0 = x0 + 1;
    }
}

