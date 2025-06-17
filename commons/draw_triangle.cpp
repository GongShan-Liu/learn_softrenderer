#include <iostream>
#include <vector>
#include "stdio.h"

#include "commons/matrix.h"
#include "commons/draw_line.h"
#include "commons/draw_triangle.h"
#include "commons/our_gl.h"

using namespace std;

const TGA_Color red = TGA_Color(255, 0, 0, 255);
const TGA_Color green = TGA_Color(0, 255, 0, 255);

// 传统线扫描法 绘制三角形
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGA_Image &image, TGA_Color color)
{
    // 按y的顺序重新排序 t2 > t1 > t0
    if (t0.y > t1.y)
        std::swap(t0, t1);
    if (t0.y > t2.y)
        std::swap(t0, t2);
    if (t1.y > t2.y)
        std::swap(t1, t2);

    // 绘制空心的三角形
    // line(t0.x, t0.y, t1.x, t1.y, image, green);
    // line(t1.x, t1.y, t2.x, t2.y, image, green);
    // line(t2.x, t2.y, t0.x, t0.y, image, red);

    /*
    // 通过水平切线切割三角形的下半部分并填充
    // 三角形点的y轴的长 = 最高y - 最低y
    int total_height = t2.y - t0.y;

    for (int y = t0.y; y <= t1.y; y++)
    {
        int segment_height = t1.y - t0.y + 1;
        // printf("y=%i", y);
        // printf("\n");

        // 求 (t0,t2)线段和(t0,t1)线段的水平位置到达长度的比率
        float alpha = (float)(y - t0.y) / total_height;
        float beta = (float)(y - t0.y) / segment_height; // be careful with divisions by zero

        // printf("a=%f", alpha);
        // printf("\n");
        // printf("b=%f", beta);
        // printf("\n");

        // 通过到达比率获取到两线段的水平位置
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = t0 + (t1 - t0) * beta;

        // printf("A.x=%i", A.x);
        // printf("\n");
        // printf("B.x=%i", B.x);
        // printf("\n");

        image.set(A.x, y, red);
        image.set(B.x, y, green);
    }
    */

    /*
    // 填充三角形的下半部分
    int total_height = t2.y - t0.y;
    for (int y = t0.y; y <= t1.y; y++)
    {
        int segment_height = t1.y - t0.y + 1; // +1是 防止t1.y == t0.y
        float alpha = (float)(y - t0.y) / total_height;
        float beta = (float)(y - t0.y) / segment_height; // be careful with divisions by zero
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = t0 + (t1 - t0) * beta;

        // 交换A和B的大小，方便在x轴从小绘制到大
        if (A.x > B.x)
            std::swap(A, B);

        for (int j = A.x; j <= B.x; j++)
        {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }

    // 填充三角形的上半部分
    for (int y = t1.y; y <= t2.y; y++)
    {
        int segment_height = t2.y - t1.y + 1;
        float alpha = (float)(y - t0.y) / total_height;
        float beta = (float)(y - t1.y) / segment_height; // be careful with divisions by zero
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = t1 + (t2 - t1) * beta;
        if (A.x > B.x)
            std::swap(A, B);
        for (int j = A.x; j <= B.x; j++)
        {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }
    */

    // 绘制2D三角形
    int total_height = t2.y - t0.y;
    for (int i = 0; i < total_height; i++)
    {
        // 通过条件来获取目前绘制的位置在三角形的上还是下
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;

        // 获得三角形的上或下的长度
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;

        // 计算迭代到达线段的比率
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here

        // 获得到达两个线段的水平位置
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;

        // 交换A和B的大小，方便在x轴从小绘制到大
        if (A.x > B.x)
            std::swap(A, B);
        for (int j = A.x; j <= B.x; j++)
        {
            image.set(j, t0.y + i, color); // attention, due to int casts t0.y+i != A.y
        }
    }
}

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P)
{
    /*** 设平面三角形ABC和一点P, 求p在三角形内坐标
     *      重心坐标: 满足u, v, w 都大于或等于0，其中 w = 1 - u - v; u + v + w = 1
     *                  u, v, w是点P关于顶点A, B, C的权重, 那么 P = (1-u-v)A + uB + vC
     *      有: 向量AB, 向量AC, 向量AP
     *      在重心坐标满足: u向量AB + v向量AC + 向量PA = 0
     *      即求: x轴(ABx, ACx, PAx)与y轴(ABy, ACy, PAy)的叉积结果
     ***/

    // 获取x轴和y轴的向量
    Vec3f s[2];
    for (int i = 2; i--;)
    {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }

    // 得到x与y的叉积结果
    Vec3f u = cross(s[0], s[1]);

    // u, v除以z轴的得到p点的位置
    if (std::abs(u[2]) > 1e-2) // u[2]是整数。如果它是零，那么三角形ABC是退化三角形(面积为零的三角形)
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);

    return Vec3f(-1, 1, 1); // 在这种情况下生成负坐标，它将被光栅化过程丢弃
}

Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P)
{
    Vec3f s[2];
    for (int i = 2; i--;)
    {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    Vec3f u = cross(s[0], s[1]);
    if (std::abs(u[2]) > 1e-2)
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return Vec3f(-1, 1, 1);
}

// 重心坐标法绘制三角形
void triangle(Vec3f *pts, TGA_Image &image, TGA_Color color)
{
    // 获取三角形的box边界
    Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            bboxmin[j] = std::max(0.0f, std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
        }
    }

    // 扫描box内的全部点，如果点在三角形内设置颜色
    Vec3f P;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                continue;

            image.set(P.x, P.y, color);
        }
    }
}

// 加入z缓冲区绘制三角形
void triangle(int &width, Vec3f *pts, float *zbuffer, TGA_Image &image, TGA_Color color)
{
    // 获取三角形的box边界
    Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width() - 1, image.get_height() - 1);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            bboxmin[j] = std::max(0.0f, std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
        }
    }

    // 扫描box内的全部点
    Vec3f P;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            // 判断点是否在三角形内
            Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                continue;

            // 通过三角面的顶点空间的Z值与P点的重心坐标的点积 计算Z缓冲值
            P.z = 0;
            for (int i = 0; i < 3; i++)
            {
                // printf("p w = %f\n", bc_screen[i]);
                P.z += pts[i][2] * bc_screen[i];
            }

            // printf("z = %f\n", P.z);
            // break;

            // 像素点记录的前Z缓冲值更小(可理解接近观察者), 那么就更新Z缓存值
            if (zbuffer[int(P.x + P.y * width)] < P.z)
            {
                zbuffer[int(P.x + P.y * width)] = P.z;
                image.set(P.x, P.y, color);
            }
        }
    }
}

void triangle(const int width, const int height, Vec3i t0, Vec3i t1, Vec3i t2, float ity0, float ity1, float ity2, TGA_Image &image, int *zbuffer)
{
    // 忽略点重叠的三角面
    if (t0.y == t1.y && t0.y == t2.y)
        return;

    // 三角面的点索引的交换 把面的点按y值的大小重新排序
    if (t0.y > t1.y)
    {
        std::swap(t0, t1);
        std::swap(ity0, ity1);
    }
    if (t0.y > t2.y)
    {
        std::swap(t0, t2);
        std::swap(ity0, ity2);
    }
    if (t1.y > t2.y)
    {
        std::swap(t1, t2);
        std::swap(ity1, ity2);
    }

    // 三角面的高
    int total_height = t2.y - t0.y;

    for (int i = 0; i < total_height; i++)
    {
        // 以t1.y来分割三角面
        bool second_half = i > t1.y - t0.y || t1.y == t0.y; // 是否在上半部分三角面

        // 上或下部分三角面的高
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;

        // 计算y轴由高到低的比率
        float alpha = (float)i / total_height;                                      // 从下到上递增值的系数
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // 上或下的三角面从下到上递增值的系数

        // 获得三角形在x轴绘制的初始点A和结束点B
        Vec3i A = t0 + Vec3f(t2 - t0) * alpha;
        Vec3i B = second_half ? t1 + Vec3f(t2 - t1) * beta : t0 + Vec3f(t1 - t0) * beta;

        // 获得三角形在x轴绘制的初始点和结束点的灯光强度
        float ityA = ity0 + (ity2 - ity0) * alpha;
        float ityB = second_half ? ity1 + (ity2 - ity1) * beta : ity0 + (ity1 - ity0) * beta;

        // 交换A和B的大小，方便在x轴从小绘制到大
        if (A.x > B.x)
        {
            std::swap(A, B);
            std::swap(ityA, ityB);
        }

        for (int j = A.x; j <= B.x; j++)
        {
            // 计算A到B在x轴的比率
            float phi = B.x == A.x ? 1. : (float)(j - A.x) / (B.x - A.x);

            // 像素点的位置向量
            Vec3i P = Vec3f(A) + Vec3f(B - A) * phi;

            // 像素点的灯光强度
            float ityP = ityA + (ityB - ityA) * phi;

            // 像素点的索引
            int idx = P.x + P.y * width;

            // 如果像素点位置不在输出画面的大小内就跳过
            if (P.x >= width || P.y >= height || P.x < 0 || P.y < 0)
                continue;

            // 像素点的Z缓存
            if (zbuffer[idx] < P.z)
            {
                zbuffer[idx] = P.z;
                image.set(P.x, P.y, TGA_Color(255, 255, 255) * ityP);
            }
        }
    }
}

void triangle(Vec4f *pts, IShader &shader, TGA_Image &image, TGA_Image &zbuffer)
{
    Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            bboxmin[j] = std::min(bboxmin[j], pts[i][j] / pts[i][3]);
            bboxmax[j] = std::max(bboxmax[j], pts[i][j] / pts[i][3]);
        }
    }
    Vec2i P;
    TGA_Color color;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            Vec3f c = barycentric(proj<2>(pts[0] / pts[0][3]), proj<2>(pts[1] / pts[1][3]), proj<2>(pts[2] / pts[2][3]), proj<2>(P));
            float z = pts[0][2] * c.x + pts[1][2] * c.y + pts[2][2] * c.z;
            float w = pts[0][3] * c.x + pts[1][3] * c.y + pts[2][3] * c.z;
            int frag_depth = std::max(0, std::min(255, int(z / w + .5)));
            if (c.x < 0 || c.y < 0 || c.z < 0 || zbuffer.get(P.x, P.y)[0] > frag_depth)
                continue;
            bool discard = shader.fragment(c, color);
            if (!discard)
            {
                zbuffer.set(P.x, P.y, TGA_Color(frag_depth));
                image.set(P.x, P.y, color);
            }
        }
    }
}