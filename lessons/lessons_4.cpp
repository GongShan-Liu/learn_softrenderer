#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "commons/tga_image.h"
#include "commons/model.h"
#include "commons/our_gl.h"
#include "commons/matrix.h"
#include "commons/commons.h"
#include "commons/draw_point.h"
#include "commons/draw_line.h"
#include "commons/draw_triangle.h"
#include "lessons/lessons_4.h"

Vec4f GouraudShader::vertex(int iface, int nthvert, Vec3f light_dir)
{
    // 面的3个顶点所对应uv空间的位置(2x3的矩阵: 2行代表u轴和v轴; 3个顶点的u和v坐标)
    varying_uv.set_col(nthvert, model->uv(iface, nthvert));

    // 计算 漫反射灯光强度插值 (面的点位置向量 点乘 灯光向量)
    varying_intensity[nthvert] = std::max(0.f, model->normal(iface, nthvert) * light_dir);

    // 获取模型面的点在世界的位置向量
    Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert));

    // 计算点的视口向量 = 视口矩阵 * 投影矩阵 * 相机矩阵 * 点世界位置向量
    gl_Vertex = Viewport * Projection * ModelView * gl_Vertex;

    return gl_Vertex;
}

bool GouraudShader::fragment(Vec3f bar, TGA_Color &color)
{
    // 计算 像素点的强度值 = 灯光强度的插值 * 像素点重心坐标
    float intensity = varying_intensity * bar;
    /*
    // 渲染效果 = 白色 * 灯光强度
    // 计算 像素点的颜色强度
    color = TGA_Color(255, 255, 255) * intensity;
    return false;
    */

    /*
    // 计算橙光按强度依次分层
    if (intensity > .85)
        intensity = 1;
    else if (intensity > .60)
        intensity = .80;
    else if (intensity > .45)
        intensity = .60;
    else if (intensity > .30)
        intensity = .45;
    else if (intensity > .15)
        intensity = .30;
    else
        intensity = 0;
    color = TGA_Color(255, 155, 0) * intensity;
    return false;
    */

    // 像素点的纹理坐标 = 面的uv坐标 点乘 重心坐标
    Vec2f uv = varying_uv * bar;

    /*
    // 渲染效果 = 漫反射贴图 * 灯光强度

    // model->diffuse(uv) = uv纹理坐标对应的漫反射贴图的像素颜色
    // 漫反射贴图的像素点颜色 乘以 像素点的强度值
    color = model->diffuse(uv) * intensity;
    */

    
    // 渲染效果 = 漫反射贴图 * 法线贴图 * 灯光强度

    // embed<4>(model->normal(uv))).normalize() 3D法线向量转为4D齐次坐标
    // 模型变换后的法线也需要变换 n 法线变换的单位向量 = uniform_MIT * 4D法线向量
    Vec3f n = proj<3>(uniform_MIT * embed<4>(model->normal(uv))).normalize();

    // 灯光变换后的单位向量
    Vec3f l = proj<3>(uniform_M * embed<4>(light_dir)).normalize();

    // 像素点的法线强度 = 法线 * 灯光
    intensity = std::max(0.f, n * l);

    // 像素点颜色 乘以 法线强度
    color = model->diffuse(uv) * intensity;
    

    return false;
}

void lessons_10(Model *model, TGA_Image &image, int width, int height)
{
    const int depth = 255;

    Vec3f light_dir(1, 1, 1);
    Vec3f eye(1, 1, 3);
    Vec3f center(0, 0, 0);
    Vec3f up(0, 1, 0);
    light_dir.normalize();

    // 计算视口矩阵
    Matrix viewport_matrix = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);

    // 计算投影矩阵
    Matrix projection_matrix = projection(-1.f / (eye - center).norm());

    // 计算相机矩阵
    Matrix lookat_matrix = lookat(eye, center, up);

    TGA_Image zbuffer(width, height, TGA_Image::GRAYSCALE);

    // 设置模型的材质
    GouraudShader shader(model, light_dir, viewport_matrix, projection_matrix, lookat_matrix);

    // 光栅化
    for (int i = 0; i < model->nfaces(); i++)
    {
        Vec4f screen_coords[3];
        for (int j = 0; j < 3; j++)
        {
            // 计算点的视口向量
            screen_coords[j] = shader.vertex(i, j, light_dir);
        }

        // 面进行光栅化
        triangle(screen_coords, shader, image, zbuffer);
    }
}
