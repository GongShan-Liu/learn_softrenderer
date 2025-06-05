#ifndef __LESSONS_1_H__
#define __LESSONS_1_H__

#include <iostream>
#include <string>
#include "stdio.h"
#include "direct.h"

#include "commons/tga_image.h"
#include "commons/model.h"
#include "commons/draw_point.h"
#include "commons/draw_line.h"
#include "commons/draw_triangle.h"

/* 绘制点 */
void lessons_1(TGA_Image &image, int x, int y);

/* 绘制直线 */
void lessons_2(TGA_Image &image);

/* 绘制线框状态的模型 */
void lessons_3(Model* model, TGA_Image &image, int width, int height);

#endif // __LESSONS_H__