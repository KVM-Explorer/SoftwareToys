#include <iostream>
#include "algriothmnTest.h"

using namespace std;



int main(int argc, char** argv)
{

    auto image = simpleWorldTest();
    image.write_tga_file("output.tga");
    return 0;
}

