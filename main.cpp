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
const TGAColor green  = TGAColor(0,   255, 0,   255);
const TGAColor blue   = TGAColor(0,   0,   255, 255);
const TGAColor yellow = TGAColor(255, 255, 0,   255);


Model *model = NULL;
const int width = 800;
const int height = 800;
const int depth  = 255;



string get_parent_path(string &path, const char *_ptr="\\")
{
    size_t pos = path.find_last_of(_ptr);
    if (pos != string::npos)
    {
        return path.substr(0, pos);
    }

    return path;
}


// 从3D转2D
Vec3f m2v(Matrix m) {
    return Vec3f(m[0][0]/m[3][0], m[1][0]/m[3][0], m[2][0]/m[3][0]);
}

// 从2D转3D
Matrix v2m(Vec3f v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}

// 3D坐标转为屏幕2D坐标
Vec3f world2screen(Vec3f v)
{
    return Vec3f(int((v.x + 1.) * width / 2.0 + 0.5), int((v.y + 1.0) * height / 2.0 + 0.5), v.z);
}

/*
// 视口就是窗口中用来显示图形的一块矩形区域，它可以和窗口等大，也可以比窗口大或者小
    
    视口矩阵就是 把2D屏幕转为3D屏幕, 最终把图形显示在屏幕3D立方体的区域
        矩阵
            | w/2  0   0  x+w/2 |
            |  0  h/2  0  y+h/2 |
            |  0   0  d/2  d/2  |
            |  0   0   0    1   |

    为什么屏幕不是2D区域而是3D立体区域？
        因为使用Z缓存区进行深度的计算, 这里的d(depth)就是Z的缓存区的分辨率, 
            depth设置为255，因为转储Z缓冲区的黑白图像以进行调试很简单。

*/
Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = depth/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth/2.f;
    return m;
}

Matrix translation(Vec3f v) {
    Matrix Tr = Matrix::identity(4);
    Tr[0][3] = v.x;
    Tr[1][3] = v.y;
    Tr[2][3] = v.z;
    return Tr;
}

Matrix zoom(float factor) {
    Matrix Z = Matrix::identity(4);
    Z[0][0] = Z[1][1] = Z[2][2] = factor;
    return Z;
}

Matrix rotation_x(float cosangle, float sinangle) {
    Matrix R = Matrix::identity(4);
    R[1][1] = R[2][2] = cosangle;
    R[1][2] = -sinangle;
    R[2][1] =  sinangle;
    return R;
}

Matrix rotation_y(float cosangle, float sinangle) {
    Matrix R = Matrix::identity(4);
    R[0][0] = R[2][2] = cosangle;
    R[0][2] =  sinangle;
    R[2][0] = -sinangle;
    return R;
}

Matrix rotation_z(float cosangle, float sinangle) {
    Matrix R = Matrix::identity(4);
    R[0][0] = R[1][1] = cosangle;
    R[0][1] = -sinangle;
    R[1][0] =  sinangle;
    return R;
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
    TGAImage image(width, height, TGAImage::RGB);

    /*
        说明: 以下每一个带有序号的注释就是课程进度的学习顺序
            1. 绘制像素点
            2. 绘制直线
            3. 绘制obj模型的线框
            4. 传统线扫描法绘制三角形
            5. 重心坐标法绘制三角形
            6. 绘制模型的三角面
            7. 光栅化模型
    */

    /*
    // 1. 绘制像素点
    
    // 设置 像素位置 (99, 0)的颜色为红色
    image.set(0, 0, red);
    image.set(98, 98, red);
    */

    /*
    // 2. 绘制直线

    line(13, 20, 80, 40, image, white);
    line(20, 13, 40, 80, image, red);
    line(80, 40, 13, 20, image, red);
    */

    // 读取obj模型
    string head_model = objs_path + "african_head.obj";
    model = new Model(head_model.c_str());

    
    // 3. 绘制obj模型的线框, 只用了三维坐标的xy轴的位置

    // for (int i = 0; i < model->nfaces(); i++)
    // {
    //     std::vector<int> face = model->face(i);
    //     for (int j = 0; j < 3; j++)
    //     {
    //         Vec3f v0 = model->vert(face[j]);
    //         Vec3f v1 = model->vert(face[(j + 1) % 3]);
    //         int x0 = (v0.x + 1.0) * width / 2.0;
    //         int y0 = (v0.y + 1.0) * height / 2.0;
    //         int x1 = (v1.x + 1.0) * width / 2.0;
    //         int y1 = (v1.y + 1.0) * height / 2.0;
    //         line(x0, y0, x1, y1, image, white);
    //     }
    // }

    
    /*
    // 4. 传统线扫描法绘制三角形

    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
    triangle(t0[0], t0[1], t0[2], image, red);
    triangle(t1[0], t1[1], t1[2], image, white);
    triangle(t2[0], t2[1], t2[2], image, red);

    */

    /*
    // 5. 重心坐标法绘制三角形
    Vec3f pts[3] = {Vec3f(10, 10, 1), Vec3f(100, 30, 1), Vec3f(190, 160, 1)};
    triangle(pts, image, red);

    */

    
    /*
    // 重心坐标法
    // 6. 绘制模型的三角面, 颜色为任意色
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
        triangle(screen_coords, image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));


    }
    */
    
    
    /*
    //  7. 光栅化模型的过程 使用Z缓冲和不使用

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


    // 加载立方体模型
    string cube_model_path = objs_path + "cube.obj";
    Model *cube_model = new Model(cube_model_path.c_str());

    // 视口矩阵
    Matrix VP = viewport(width/200.0, width/200.0, width, height);

    { // 绘制2D的中心坐标
        Vec3f x(1.0f, 0.f, 0.f), y(0.0f, 1.0f, 0.f), o(0.f, 0.f, 0.f);


        o = m2v(VP * v2m(o));
        x = m2v(VP * v2m(x));
        y = m2v(VP * v2m(y));


        line(o, x, image, red);
        line(o, y, image, green);
    }


    // for (int i=0; i < cube_model->nfaces(); i++) 
    // {
    //     std::vector<int> face = cube_model->face(i);

    //     for (int j=0; j<(int)face.size(); j++) 
    //     {
    //         Vec3f wp0 = cube_model->vert(face[j]);
    //         Vec3f wp1 = cube_model->vert(face[(j+1) % face.size()]);

    //         { // 绘制原始模型(作为参照物)
    //             Vec3f sp0 = m2v(VP * v2m(wp0));
    //             Vec3f sp1 = m2v(VP * v2m(wp1));
    //             line(sp0, sp1, image, white);
    //         }


    //         // { // 绘制变换后的模型
    //         //     Matrix T = zoom(1.5);
    //         //     // Matrix T = Matrix::identity(4);
    //         //     // T[0][1] = 0.333;
    //         //     // Matrix T = translation(Vec3f(.33, .5, 0))*rotation_z(cos(10.*M_PI/180.), sin(10.*M_PI/180.));
    //         //     Vec3f sp0 = m2v(VP * T * v2m(wp0));
    //         //     Vec3f sp1 = m2v(VP * T * v2m(wp1));
    //         //     line(sp0, sp1, image, yellow);
    //         // }
    //     }
    //     break;
    // }

    delete cube_model;

    

    // 清理内存
    delete model;

    // 设置垂直翻转
    image.flip_vertically();

    // 把图片数据写入到tgaw文件
    string image_path = output_images_path + "render.tga";
    image.write_tga_file(image_path.c_str());



    return 0;
}