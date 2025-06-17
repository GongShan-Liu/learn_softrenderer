#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "commons/tga_image.h"
#include "commons/model.h"
#include "commons/draw_point.h"
#include "commons/draw_line.h"
#include "commons/draw_triangle.h"
#include "commons/commons.h"

#include "lessons/lessons_3.h"

const TGA_Color white = TGA_Color(255, 255, 255, 255);
const TGA_Color red = TGA_Color(255, 0, 0, 255);
const TGA_Color green = TGA_Color(0, 255, 0, 255);
const TGA_Color yellow = TGA_Color(255, 255, 0, 255);

const int depth = 255;

void lessons_8(string &objs_path, TGA_Image &image, int width, int height)
{

    // 加载立方体模型
    string cube_model_path = objs_path + "cube.obj";
    Model *cube_model = new Model(cube_model_path.c_str());

    // 视口矩阵
    Matrix ViewPort = viewport(width / 4, width / 4, width / 2, height / 2, depth);

    // 绘制2D的中心坐标
    Vec3f x(1.0f, 0.f, 0.f), y(0.0f, 1.0f, 0.f), o(0.f, 0.f, 0.f);

    // 转换为视口的坐标
    x = m2v(ViewPort * v2m(x));
    y = m2v(ViewPort * v2m(y));
    o = m2v(ViewPort * v2m(o));

    draw_line(o, x, image, red);
    draw_line(o, y, image, green);

    for (int i = 0; i < cube_model->nfaces(); i++)
    {
        std::vector<int> face = cube_model->face(i);

        for (int j = 0; j < (int)face.size(); j++)
        {
            Vec3f wp0 = cube_model->vert(face[j]);
            Vec3f wp1 = cube_model->vert(face[(j + 1) % face.size()]);

            // 绘制原始模型(作为参照物)
            Vec3f sp0 = m2v(ViewPort * v2m(wp0));
            Vec3f sp1 = m2v(ViewPort * v2m(wp1));
            draw_line(sp0, sp1, image, white);

            // 绘制变换后的模型
            {
                // 1. 缩放
                Matrix T = zoom(1.5);

                /*
                // 2. 剪切
                Matrix T = Matrix::identity();
                T[0][1] = 0.333;
                */

                /*
                // 3.先旋转后位移
                Matrix T = translation(Vec3f(0.33, 0.5, 0.0)) * rotation_z(cos(10.*M_PI/180.), sin(10.*M_PI/180.));
                */

                Vec3f sp0 = m2v(ViewPort * T * v2m(wp0));
                Vec3f sp1 = m2v(ViewPort * T * v2m(wp1));
                draw_line(sp0, sp1, image, yellow);
            }
        }
        break;
    }

    delete cube_model;
}

void lessons_9(Model *model, TGA_Image &image, int width, int height, string &output_images_path)
{
    // 灯光位置
    Vec3f light_dir = Vec3f(1, -1, 1).normalize(); // 向量归一化

    // 相机位置
    Vec3f eye(1, 1, 3);

    // 中心位置
    Vec3f center(0, 0, 0); // 

    // z缓存数据
    int *zbuffer = NULL;
    zbuffer = new int[width * height];
    for (int i = 0; i < width * height; i++)
    {
        zbuffer[i] = std::numeric_limits<int>::min();
    }

    // draw the model
    
    // 相机矩阵 
    Matrix ModelView = lookat(eye, center, Vec3f(0, 1, 0));

    // 投影矩阵
    Matrix Projection = Matrix::identity();

    Projection[3][2] = -1.f / (eye - center).norm(); // w透视

    // 视口矩阵
    Matrix ViewPort = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);

    std::cout << "相机矩阵: " << std::endl;
    std::cerr << ModelView << std::endl;

    std::cout << "投影矩阵: " << std::endl;
    std::cerr << Projection << std::endl;

    std::cout << "视口矩阵: " << std::endl;
    std::cerr << ViewPort << std::endl;

    // 计算3D位置的矩阵算法
    std::cout << "z: " << std::endl;
    Matrix z = (ViewPort * Projection * ModelView);

    std::cerr << z << std::endl;

    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);

        // 屏幕坐标
        Vec3i screen_coords[3];

        // 世界坐标
        Vec3f world_coords[3];
        
        // 灯光强度
        float intensity[3];

        for (int j = 0; j < 3; j++)
        {
            Vec3f v = model->vert(face[j]);

            // 每个面的三个顶点的视口坐标
            screen_coords[j] = Vec3f(ViewPort * Projection * ModelView * Matrix(v));

            // 每个面的三个顶点的世界坐标
            world_coords[j] = v;

            // 计算每个点的灯光强度 = 法线 * 灯光 (法线向量和灯光向量的点乘结果)
            intensity[j] = model->normal(i, j) * light_dir; 
        }
        
        // 光栅化
        triangle(width, height, screen_coords[0], screen_coords[1], screen_coords[2], intensity[0], intensity[1], intensity[2], image, zbuffer);
    }

    // 渲染Z通道
    { 
        TGA_Image zbimage(width, height, TGA_Image::GRAYSCALE);

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                zbimage.set(i, j, TGA_Color(zbuffer[i + j * width]));
            }
        }

        zbimage.flip_vertically();
        string image_path = output_images_path + "zbuffer.tga";
        zbimage.write_tga_file(image_path.c_str());
    }

    delete[] zbuffer;
}