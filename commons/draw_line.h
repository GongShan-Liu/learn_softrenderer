#ifndef __DRAW_LINE_H__
#define __DRAW_LINE_H__

#include "commons/tga_image.h"
#include "commons/matrix.h"

/***
绘制直线算法，意思就是 在一个 n*m的像素(可理解为网格)中 找到 点A(x0, y0) 连接到 点B(x1, y1) 的最短距离的网格

    例如在 5*5 中找到 点A(0,0) 到 点B(4, 4) 的网格

      (4,0) #     #     #     B
      (3,0) #     #     #     #
      (2,0) #     #     #     #
      (1,0) #     #     #     #
        A (0,1) (0,2) (0,3) (0,4)
***/

void draw_line(int x0, int y0, int x1, int y1, TGA_Image &image, TGA_Color color);

void draw_line(Vec3f p0, Vec3f p1, TGA_Image &image, TGA_Color color);

#endif