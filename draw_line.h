#ifndef __DRAW_LINE_H__
#define __DRAW_LINE_H__

#include "tgaimage.h"
#include "geometry.h"

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

void line(Vec3f p0, Vec3f p1, TGAImage &image, TGAColor color);

#endif