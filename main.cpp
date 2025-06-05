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

using namespace std;

const TGA_Color white = TGA_Color(255, 255, 255, 255);
const TGA_Color red = TGA_Color(255, 0, 0, 255);
const TGA_Color green = TGA_Color(0, 255, 0, 255);
const TGA_Color blue = TGA_Color(0, 0, 255, 255);
const TGA_Color yellow = TGA_Color(255, 255, 0, 255);

Model *model = NULL;
const int width = 800;
const int height = 800;
const int depth = 255;

int *zbuffer = NULL;
Vec3f light_dir = Vec3f(1, -1, 1).normalize();
Vec3f eye(1, 1, 3);
Vec3f center(0, 0, 0);

string get_parent_path(string &path, const char *_ptr = "\\")
{
    size_t pos = path.find_last_of(_ptr);
    if (pos != string::npos)
    {
        return path.substr(0, pos);
    }

    return path;
}

// 从3D转2D
Vec3f m2v(Matrix m)
{
    return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

// 从2D转3D
Matrix v2m(Vec3f v)
{
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
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
Matrix viewport(int x, int y, int w, int h)
{
    Matrix m = Matrix::identity(4);
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = depth / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = depth / 2.f;
    return m;
}

Matrix translation(Vec3f v)
{
    Matrix Tr = Matrix::identity(4);
    Tr[0][3] = v.x;
    Tr[1][3] = v.y;
    Tr[2][3] = v.z;
    return Tr;
}

Matrix zoom(float factor)
{
    Matrix Z = Matrix::identity(4);
    Z[0][0] = Z[1][1] = Z[2][2] = factor;
    return Z;
}

Matrix rotation_x(float cosangle, float sinangle)
{
    Matrix R = Matrix::identity(4);
    R[1][1] = R[2][2] = cosangle;
    R[1][2] = -sinangle;
    R[2][1] = sinangle;
    return R;
}

Matrix rotation_y(float cosangle, float sinangle)
{
    Matrix R = Matrix::identity(4);
    R[0][0] = R[2][2] = cosangle;
    R[0][2] = sinangle;
    R[2][0] = -sinangle;
    return R;
}

Matrix rotation_z(float cosangle, float sinangle)
{
    Matrix R = Matrix::identity(4);
    R[0][0] = R[1][1] = cosangle;
    R[0][1] = -sinangle;
    R[1][0] = sinangle;
    return R;
}

Matrix lookat(Vec3f eye, Vec3f center, Vec3f up)
{
    Vec3f z = (eye - center).normalize();
    Vec3f x = (up ^ z).normalize();
    Vec3f y = (z ^ x).normalize();
    Matrix res = Matrix::identity(4);
    for (int i = 0; i < 3; i++)
    {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[i][3] = -center[i];
    }
    return res;
}

int main(int argc, char **argv)
{ 
    std::cout << "hello world" << std::endl;
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
    */

    /*
    // 1. 绘制像素点
    // 设置 像素位置 中心的颜色为红色
    // lessons_1(image, width / 2, height / 2);
    */

    
    // 2. 绘制直线
    lessons_2(image);
    

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
    //  7. 光栅化模型的过程 使用Z缓冲和不使用
    lessons_7(model, image, width, height);
    */

    /*
    // 8. 下面代码用以测试物体的变换来理解透视投影

    // 绘制白色物体（一个角被切掉的白色正方形）将变换为黄色物体。红黄色为坐标 (红色和绿色线段分别给出与 x 和 y 轴对齐的单位长度向量)

    // 加载立方体模型
    string cube_model_path = objs_path + "cube.obj";
    Model *cube_model = new Model(cube_model_path.c_str());

    // 视口矩阵
    Matrix VP = viewport(width/4, width/4, width/2, height/2);

    { // 绘制2D的中心坐标
        Vec3f x(1.0f, 0.f, 0.f), y(0.0f, 1.0f, 0.f), o(0.f, 0.f, 0.f);


        o = m2v(VP * v2m(o));
        x = m2v(VP * v2m(x));
        y = m2v(VP * v2m(y));


        draw_line(o, x, image, red);
        draw_line(o, y, image, green);
    }


    for (int i=0; i < cube_model->nfaces(); i++)
    {
        std::vector<int> face = cube_model->face(i);

        for (int j=0; j<(int)face.size(); j++)
        {
            Vec3f wp0 = cube_model->vert(face[j]);
            Vec3f wp1 = cube_model->vert(face[(j+1) % face.size()]);

            { // 绘制原始模型(作为参照物)
                Vec3f sp0 = m2v(VP * v2m(wp0));
                Vec3f sp1 = m2v(VP * v2m(wp1));
                draw_line(sp0, sp1, image, white);
            }


            { // 绘制变换后的模型
                Matrix T = zoom(1.5);
                // Matrix T = Matrix::identity(4);
                // T[0][1] = 0.333;
                // Matrix T = translation(Vec3f(.33, .5, 0))*rotation_z(cos(10.*M_PI/180.), sin(10.*M_PI/180.));
                Vec3f sp0 = m2v(VP * T * v2m(wp0));
                Vec3f sp1 = m2v(VP * T * v2m(wp1));
                draw_line(sp0, sp1, image, yellow);
            }
        }
        break;
    }

    delete cube_model;

    */

    // // 9.
    // zbuffer = new int[width*height];
    // for (int i=0; i<width*height; i++) {
    //     zbuffer[i] = std::numeric_limits<int>::min();
    // }

    // // draw the model
    // Matrix ModelView  = lookat(eye, center, Vec3f(0,1,0));
    // Matrix Projection = Matrix::identity(4);
    // Matrix ViewPort   = viewport(width/8, height/8, width*3/4, height*3/4);
    // Projection[3][2] = -1.f/(eye-center).norm();

    // std::cerr << ModelView << std::endl;
    // std::cerr << Projection << std::endl;
    // std::cerr << ViewPort << std::endl;
    // Matrix z = (ViewPort*Projection*ModelView);
    // std::cerr << z << std::endl;

    // TGA_Image image(width, height, TGA_Image::RGB);
    // for (int i=0; i<model->nfaces(); i++) {
    //     std::vector<int> face = model->face(i);
    //     Vec3i screen_coords[3];
    //     Vec3f world_coords[3];
    //     float intensity[3];
    //     for (int j=0; j<3; j++) {
    //         Vec3f v = model->vert(face[j]);
    //         screen_coords[j] =  Vec3f(ViewPort*Projection*ModelView*Matrix(v));
    //         world_coords[j]  = v;
    //         intensity[j] = model->norm(i, j)*light_dir;
    //     }
    //     triangle(width, height,  screen_coords[0], screen_coords[1], screen_coords[2], intensity[0], intensity[1], intensity[2], image, zbuffer);
    // }

    // { // dump z-buffer (debugging purposes only)
    //     TGA_Image zbimage(width, height, TGA_Image::GRAYSCALE);
    //     for (int i=0; i<width; i++) {
    //         for (int j=0; j<height; j++) {
    //             zbimage.set(i, j, TGA_Color(zbuffer[i+j*width]));
    //         }
    //     }
    //     zbimage.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    //     string image_path = output_images_path + "zbuffer.tga";
    //     zbimage.write_tga_file(image_path.c_str());
    // }

    // 清理内存
    delete model;
    delete[] zbuffer;

    // 设置垂直翻转
    image.flip_vertically();

    // 把图片数据写入到tgaw文件
    string image_path = output_images_path + "render.tga";
    image.write_tga_file(image_path.c_str());

    return 0;
}