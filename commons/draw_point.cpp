#include <iostream>

#include "commons/tga_image.h"
#include "commons/draw_point.h"

// 绘制像素点 就是在图像上面某个像素位置上会制颜色
void draw_point(TGA_Image &image, int x, int y, TGA_Color color)
{
    image.set(x, y, color);
}