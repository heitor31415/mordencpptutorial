#include <image.h>
#include <iostream>
#include <string>

int main()
{
    igg::Image demo(500, 500);
    demo.FillFromPgm("data/lena.ascii.pgm");
    std::cout << "The image has " << demo.rows() << " rows and " << demo.cols() << " cols";
    return 0;
}