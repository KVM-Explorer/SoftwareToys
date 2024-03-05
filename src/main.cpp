#include <iostream>
#include "algriothmnTest.h"

using namespace std;



int main(int argc, char** argv)
{

  auto image = GouraudShaderTest();
  image.flip_vertically(); // 变换后的物体上下颠倒
    image.write_tga_file("output.tga");
    return 0;
}

