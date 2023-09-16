#include <iostream>
#include <string>
#include "stdio.h"
#include "direct.h"

#include "tgaimage.h"
#include "model.h"

#include "draw_line.h"
#include "draw_triangle.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);


Model *model;
const int width = 800;
const int height = 800;

Vec3f world2screen(Vec3f v)
{
    return Vec3f(int((v.x + 1.) * width / 2.0 + 0.5), int((v.y + 1.0) * height / 2.0 + 0.5), v.z);
}

string get_parent_path(string &path, const char *_ptr="\\")
{
    size_t pos = path.find_last_of(_ptr);
    if (pos != string::npos)
    {
        return path.substr(0, pos);
    }

    return path;
}

int main(int argc, char **argv)
{
    // exe 路径
    char _exe_path[250];
    getcwd(_exe_path, 250);

    // 工程各部分路径
    string build_path = get_parent_path(string(_exe_path));
    string pro_path = get_parent_path(build_path);
    string objs_path = pro_path + "\\objs\\";
    string output_images_path = pro_path + "\\outpur_images\\";

    // std::cout << "path: = " << objs_path << std::endl;

    std::cout << "start running..." << std::endl;

    // 设置 tga图片的像素大小 100就是 0-99个像素格子 在ps可查看
    TGAImage image(width, height, TGAImage::RGB);

    /*
    // 绘制像素点
    
    // 设置 像素位置 (99, 0)的颜色为红色
    image.set(0, 0, red);
    image.set(98, 98, red);
    */

    /*
    // 绘制直线

    line(13, 20, 80, 40, image, white);
    line(20, 13, 40, 80, image, red);
    line(80, 40, 13, 20, image, red);
    */

    // 读取obj模型
    string head_model = objs_path + "african_head.obj";
    model = new Model(head_model.c_str());

    /*
    // 绘制obj模型的线框, 只用了三维坐标的xy轴的位置

    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++)
        {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            int x0 = (v0.x + 1.0) * width / 2.0;
            int y0 = (v0.y + 1.0) * height / 2.0;
            int x1 = (v1.x + 1.0) * width / 2.0;
            int y1 = (v1.y + 1.0) * height / 2.0;
            line(x0, y0, x1, y1, image, white);
        }
    }
    */
    
    /*
    // 传统线扫描法绘制三角形

    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
    triangle(t0[0], t0[1], t0[2], image, red);
    triangle(t1[0], t1[1], t1[2], image, white);
    triangle(t2[0], t2[1], t2[2], image, red);

    */

    /*
    // 重心坐标法绘制三角形
    Vec3f pts[3] = {Vec3f(10, 10, 1), Vec3f(100, 30, 1), Vec3f(190, 160, 1)};
    triangle(pts, image, red);

    */

    /*

    // 重心坐标法
    // 绘制模型的三角面, 颜色为任意色
    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        Vec3f screen_coords[3];
        for (int j = 0; j < 3; j++)
        {
            // 获取模型面的世界坐标
            Vec3f world_coords = model->vert(face[j]);
            // 从世界坐标转换到屏幕坐标 (简单地去掉z轴)
            screen_coords[j] = Vec3f((world_coords.x + 1.0) * width / 2.0, (world_coords.y + 1.0) * height / 2.0, 1.0);
        }
        triangle(screen_coords, image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
    }

    */
    
    /*
    //  光栅化模型的过程 使用Z缓冲和不使用

    // 定义一个灯光
    Vec3f light_dir(0, 0, -1);

    float *zbuffer = new float[width * height];
    for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        Vec3f screen_coords[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; j++)
        {
            Vec3f v = model->vert(face[j]);
            // screen_coords[j] = Vec3f((v.x + 1.0) * width / 2.0, (v.y + 1.0) * height / 2.0, 1.0);
            screen_coords[j] = world2screen(v);
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
            // triangle(screen_coords, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));

            // 使用Z缓冲
            int w = width;
            triangle(w, screen_coords, zbuffer, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
        }
    }
    
    delete zbuffer;
    */
    

    // 清理内存
    delete model;

    // 设置垂直翻转
    image.flip_vertically();

    // 把图片数据写入到tgaw文件
    string image_path = output_images_path + "render.tga";
    image.write_tga_file(image_path.c_str());



    return 0;
}