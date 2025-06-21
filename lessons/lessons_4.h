#ifndef __LESSONS_4_H__
#define __LESSONS_4_H__

#include <iostream>
#include <string>

#include "commons/tga_image.h"
#include "commons/model.h"
#include "commons/our_gl.h"
#include "commons/matrix.h"
#include "commons/draw_point.h"
#include "commons/draw_line.h"
#include "commons/draw_triangle.h"

struct GouraudShader : public IShader
{
    Model *model;
    Vec3f varying_intensity;

    Matrix Viewport;
    Matrix Projection;
    Matrix ModelView;
    GouraudShader(Model *inModel,
                  Matrix inModelView,
                  Matrix inViewport,
                  Matrix inProjection) : model(inModel),
                                         Viewport(inViewport),
                                         Projection(inProjection),
                                         ModelView(inModelView)
    {
    }

    virtual Vec4f vertex(int iface, int nthvert, Vec3f light_dir);

    // 计算像素点的颜色 bar = 像素点的重心向量
    virtual bool fragment(Vec3f bar, TGA_Color &color);
};

void lessons_10(Model *model, TGA_Image &image, int width, int height);

#endif // __LESSONS_4_H__