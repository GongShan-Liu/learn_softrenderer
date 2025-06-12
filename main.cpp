#include <iostream>
#include <string>
#include "stdio.h"
#include "direct.h"

#include "commons/tga_image.h"
#include "commons/model.h"
#include "commons/draw_point.h"
#include "commons/draw_line.h"
#include "commons/draw_triangle.h"

#include "lessons/lessons_1.h"
#include "lessons/lessons_2.h"
#include "lessons/lessons_3.h"

using namespace std;

Model *model = NULL;
const int width = 800;
const int height = 800;
const int depth = 255;

string get_parent_path(string &path, const char *_ptr = "\\")
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
    // exe文件路径
    // std::cout << argv[0] << std::endl;

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
    TGA_Image image(width, height, TGA_Image::RGB);

    /*
        说明: 以下每一个带有序号的注释就是课程进度的学习顺序
            1. 绘制像素点
            2. 绘制直线
            3. 绘制obj模型的线框
            4. 传统线扫描法绘制三角形
            5. 重心坐标法绘制三角形
            6. 绘制模型的三角面
            7. 光栅化模型
            8. 用物体的变换来理解透视投影的原理
            9. 绘制带相机视角和灯光的模型
    */

    
    // 1. 绘制像素点
    // 设置 像素位置 中心的颜色为红色
    // lessons_1(image, width / 2, height / 2);
    

    /*
    // 2. 绘制直线
    lessons_2(image);
    */

    // 读取obj模型
    string head_model = objs_path + "african_head.obj";
    model = new Model(head_model.c_str());

    /*
    // 3. 绘制obj模型的线框, 只用了三维坐标的xy轴的位置
    lessons_3(model, image, width, height);
    */

    /*
    // 4. 传统线扫描法绘制三角形
    lessons_4(image);
    */

    /*
    // 5. 重心坐标法绘制三角形
    lessons_5(image);
    */

    /*
    // 重心坐标法
    // 6. 绘制模型的三角面, 颜色为任意色
    lessons_6(model, image, width, height);
    */

    /*
    // 7. 光栅化模型的过程 使用Z缓冲和不使用
    lessons_7(model, image, width, height);
    */

    /*
    // 8. 用物体的变换来理解透视投影的原理
    lessons_8(objs_path, image, width, height);
    */

    
    // 9. 绘制带相机视角和灯光的模型
    lessons_9(model, image, width, height, output_images_path);
    
    

    // 清理内存
    delete model;

    // 设置垂直翻转
    image.flip_vertically();

    // 把图片数据写入到tgaw文件
    string image_path = output_images_path + "render.tga";
    image.write_tga_file(image_path.c_str());

    return 0;
}