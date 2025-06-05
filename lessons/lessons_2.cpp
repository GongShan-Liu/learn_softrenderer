#include <iostream>
#include <string>
#include "stdio.h"
#include "direct.h"

#include "commons/tga_image.h"
#include "commons/model.h"
#include "commons/draw_point.h"
#include "commons/draw_line.h"
#include "commons/draw_triangle.h"

#include "lessons/lessons_2.h"

const TGA_Color white = TGA_Color(255, 255, 255, 255);
const TGA_Color red = TGA_Color(255, 0, 0, 255);

// 3D坐标转为屏幕2D坐标
Vec3f world2screen(Vec3f v, int width, int height)
{
    return Vec3f(int((v.x + 1.) * width / 2.0 + 0.5), int((v.y + 1.0) * height / 2.0 + 0.5), v.z);
}

void lessons_4(TGA_Image &image)
{
    Vec2i t0[3] = {Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
    triangle(t0[0], t0[1], t0[2], image, red);
    triangle(t1[0], t1[1], t1[2], image, white);
    triangle(t2[0], t2[1], t2[2], image, red);
}

void lessons_5(TGA_Image &image)
{
    Vec3f pts[3] = {Vec3f(10, 10, 1), Vec3f(100, 30, 1), Vec3f(190, 160, 1)};
    triangle(pts, image, red);
}

void lessons_6(Model *model, TGA_Image &image, int width, int height)
{
    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        Vec3f screen_coords[3];
        for (int j = 0; j < 3; j++)
        {
            // 获取模型面的世界坐标 (坐标都是在-1.0 到 1.0 之间)
            Vec3f world_coords = model->vert(face[j]);

            // 从世界坐标转换到屏幕2D坐标 (简单地去掉z轴)
            // 屏幕 (x, y) = ((3D位置.x + 1.0) * 宽 / 2 , (3D位置.y + 1.0) * 高 / 2)
            //  为什么宽和高要缩小一半？
            //      因为要把3D位置的x,y是(-1.0到1.0)) 的最大距离是2, 而2D屏幕是从0开始最大值,所以转换到屏幕要缩小一半
            //  为什么位置要 + 1.0 ？
            //      因为绘制的初始地方在 屏幕2D的(0, 0)开始绘制 所以可以理解为 3D模型要整体位移最大距离的一半(因为屏幕缩小了一半, 相应位移最大距离的一半即可)

            screen_coords[j] = Vec3f((world_coords.x + 1.0) * width / 2.0, (world_coords.y + 1.0) * height / 2.0, 1.0);
        }

        // 任意颜色
        triangle(screen_coords, image, TGA_Color(rand() % 255, rand() % 255, rand() % 255, 255));
    }
}

void lessons_7(Model *model, TGA_Image &image, int width, int height)
{
    // 定义一个灯光
    Vec3f light_dir(0, 0, -1);

    float *zbuffer = new float[width * height];
    for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max())
        ;

    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        Vec3f screen_coords[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; j++)
        {
            Vec3f v = model->vert(face[j]);
            // screen_coords[j] = Vec3f((v.x + 1.0) * width / 2.0, (v.y + 1.0) * height / 2.0, 1.0);
            screen_coords[j] = world2screen(v, width, height);
            world_coords[j] = v;
        }

        // 计算出面的法线
        Vec3f n = cross(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
        n.normalize();

        // 计算灯光强度
        float intensity = n * light_dir;

        // 光栅化
        if (intensity > 0)
        {
            // 没有Z缓冲
            // triangle(screen_coords, image, TGA_Color(intensity * 255, intensity * 255, intensity * 255, 255));

            // 使用Z缓冲
            int w = width;
            triangle(w, screen_coords, zbuffer, image, TGA_Color(intensity * 255, intensity * 255, intensity * 255, 255));
        }
    }

    delete zbuffer;
}
