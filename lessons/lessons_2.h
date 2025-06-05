#ifndef __LESSONS_2_H__
#define __LESSONS_2_H__

#include <iostream>
#include <string>
#include "stdio.h"
#include "direct.h"

#include "commons/tga_image.h"
#include "commons/model.h"
#include "commons/draw_point.h"
#include "commons/draw_line.h"
#include "commons/draw_triangle.h"

/* 传统线扫描法绘制三角形 */
void lessons_4(TGA_Image &image);

/* 重心坐标法绘制三角形 */
void lessons_5(TGA_Image &image);

/* 绘制模型的三角面, 颜色为任意色 */
void lessons_6(Model *model, TGA_Image &image, int width, int height);

/* 光栅化模型的过程可使用或不用Z缓冲 */
void lessons_7(Model *model, TGA_Image &image, int width, int height);

#endif // __LESSONS_H__