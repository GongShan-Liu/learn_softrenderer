#ifndef __DRAW_TRIANGLE_H__
#define __DRAW_TRIANGLE_H__

#include "commons/matrix.h"
#include "commons/model.h"
#include "commons/tga_image.h"
#include "commons/our_gl.h"


// 计算点的重心坐标
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);

Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P);

// 传统线扫描法绘制三角形
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGA_Image &image, TGA_Color color);

// 重心坐标法绘制三角形
void triangle(Vec3f *pts, TGA_Image &image, TGA_Color color);

// 加入z缓冲区
void triangle(int &width, Vec3f *pts, float *zbuffer, TGA_Image &image, TGA_Color color);

// 使用传统扫描法绘制三角形和z缓存、灯光
void triangle(const int width, const int height, Vec3i t0, Vec3i t1, Vec3i t2, float ity0, float ity1, float ity2, TGA_Image &image, int *zbuffer);

// 
void triangle(Vec4f *pts, IShader &shader, TGA_Image &image, TGA_Image &zbuffer);

#endif