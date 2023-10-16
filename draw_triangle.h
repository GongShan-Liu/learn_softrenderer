#ifndef __DRAW_TRIANGLE_H__
#define __DRAW_TRIANGLE_H__

#include "geometry.h"
#include "tgaimage.h"


// 计算点的重心坐标
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);

// 传统线扫描法绘制三角形
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color);

// 重心坐标法绘制三角形
void triangle(Vec3f *pts, TGAImage &image, TGAColor color);

// 加入z缓冲区
void triangle(int &width, Vec3f *pts, float *zbuffer, TGAImage &image, TGAColor color);

#endif