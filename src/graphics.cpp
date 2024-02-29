#include "graphics.h"
#include "iostream"
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

    int eps_sum = 0;
    int y = y0;
    while(x0 < x1)
    {
        if(is_reverse)
        {
            image.set(y, x0, color);
        }else{
            image.set(x0, y, color);
        }
        eps_sum += eps;
        if (eps_sum>dx){            // 因为其只关心增长速度不关心增长方向，为避免小数使用*2转化为对应的原始分母
            y += (y1 > y0) ? 1 : -1;
            eps_sum -= 2*dx;        // 注意是两倍的dx
        }
        x0 = x0 + 1;
    }
}

void drawLineModel(Model &model,TGAImage &image,TGAColor color)
{
    int width = image.get_width();
    int height = image.get_height();
    for (int i = 0; i < model.nfaces();i++)
    {
        std::vector<int> face = model.face(i);
        for (int j = 0; j < 3;j++)
        {
            Vec3f v0 = model.vert(face[j]);
            Vec3f v1 = model.vert(face[(j + 1) % 3]);

            int x0 = (v0.x + 1.0F) * width/2.0F;
            int y0 = (v0.y + 1.0F) * height/2.0F;
            int x1 = (v1.x + 1.0F) * width/2.0F;
            int y1 = (v1.y + 1.0F) * height/2.0F;
            lineV3(x0, y0, x1, y1, image, color);
        }
    }
    
}

void flatShadingModel(Model &model, TGAImage &image)
{
    srand((unsigned)time(NULL)); 

    int width = image.get_width();
    int height = image.get_height();
    for (int i = 0; i < model.nfaces();i++)
    {
        std::vector<int> face = model.face(i);
        TGAColor color = TGAColor(rand() % 255, rand() % 255, rand() % 255, 255);
        std::vector<Vec2i> verts{};
        for (int j = 0; j < 3;j++)
        {
            Vec3f v0 = model.vert(face[j]);

            int x0 = (v0.x + 1.0F) * width/2.0F;
            int y0 = (v0.y + 1.0F) * height/2.0F;

            verts.push_back({x0,y0,});
        }
        // boundingboxTriange({verts[0],verts[1],verts[2]}, image, color);
        scanTriangle(verts[0], verts[1], verts[2], image, color);
    }
    
}

void triangleLine(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color)
{
    lineV3(t0.x, t0.y, t1.x, t1.y, image, color);
    lineV3(t1.x, t1.y, t2.x, t2.y, image, color);
    lineV3(t2.x, t2.y, t0.x, t0.y, image, color);
}

void scanTriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color)
{
    if(t0.y == t1.y && t0.y == t2.y)
        return;
    // t0 < t1 < t2
    if(t0.y>t1.y) std::swap(t0, t1);
    if(t1.y>t2.y) std::swap(t1, t2);
    if(t0.y>t1.y) std::swap(t0, t1);

    int height = t2.y - t0.y;
    int half_height = t1.y - t0.y;
    for (int i = 0; i <= height;i++)
    {
        float alpha = float(i) / float(t2.y - t0.y);// t0 - t2
        int left = alpha * float(t2.x - t0.x) + t0.x;

        if(left<0) {
            int x = 1;
        }
        if(i <= half_height && t1.y != t0.y){
            float beta = float(i) / float(t1.y - t0.y); // t0 - t1

            int right = beta * float(t1.x - t0.x) + t0.x;


            if(left> right) std::swap(right, left);

            for (int j = left; j <= right;j++){
                image.set(j, i + t0.y, color);
            }
        }else{
            float beta = float(i - half_height) / float(t2.y - t1.y);
            int right = beta * float(t2.x - t1.x) + t1.x;
            if(left > right ) std:: swap(left, right);
            for (int j = left; j <= right;j++){
                image.set(j, i + t0.y, color);
            }
        }
    }
}

// TODO: Fix BoundingBoxTriangle Bug
Vec3f barycentric(std::array<Vec2i,3> verts , Vec2i P)
{
    // 向量叉乘判定点在内外侧
   Vec3f u = Vec3f(
    verts[2].x-verts[0].x, 
    verts[1].x-verts[0].x, 
    verts[0].x-P.x)^Vec3f(
        verts[2].y-verts[0].y, 
        verts[1].y-verts[0].y, 
        verts[0].y-P.y);
   if (std::abs(u.z)<1) return Vec3f(-1,1,1);
   return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z); 
}

void boundingboxTriange(std::array<Vec2i,3> verts, TGAImage &image, TGAColor color)
{
    if(verts[0].y == verts[1].y && verts[1].y == verts[2].y)
        return;
    Vec2i bboxmin(image.get_width()-1, image.get_height()-1);
    Vec2i bboxmax(0, 0);

    Vec2i clamp(image.get_width() - 1, image.get_height() - 1);
    for (int i = 0;i<3;i++)
    {
        bboxmin.x = std::min(bboxmin.x, verts[i].x);
        bboxmin.y = std::min(bboxmin.y, verts[i].y);

        bboxmax.x = std::max(bboxmax.x, verts[i].x);
        bboxmax.y = std::max(bboxmax.y, verts[i].y);

        // 防止溢出
        bboxmin.x = std::max(0, bboxmin.x); 
        bboxmin.y = std::max(0, bboxmin.y);

        bboxmax.x = std::min(clamp.x, verts[i].x);
        bboxmax.y = std::min(clamp.y, verts[i].y);
    }

    for (int i = bboxmin.x; i <= bboxmax.x;i++)
    {
        for (int j = bboxmin.y; j <= bboxmax.y;j++)
        {
            // Vector X
            // barycentric

            Vec3f p = barycentric(verts, {i, j});
            if (p.x<0 || p.y<0 || p.z<0) continue; 
            image.set(i,j, color); 
        }
    }
}
