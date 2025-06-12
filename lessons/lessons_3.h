#ifndef __LESSONS_3_H__
#define __LESSONS_3_H__

#include <iostream>
#include <string>

#include "commons/tga_image.h"
#include "commons/model.h"
#include "commons/draw_point.h"
#include "commons/draw_line.h"
#include "commons/draw_triangle.h"

using namespace std;

/* 
绘制白色物体（一个角被切掉的白色正方形）将变换为黄色物体。
  红绿色为中心坐标 (红色和绿色线段分别给出与 x 和 y 轴对齐的单位长度向量) 
*/
void lessons_8(string &objs_path, TGA_Image &image, int width, int height);

/* 绘制以相机视角带光源的模型 */
void lessons_9(Model *model, TGA_Image &image, int width, int height, string &output_images_path);

#endif // __LESSONS_3_H__