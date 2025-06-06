#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>

#include "commons/matrix.h"
#include "commons/commons.h"

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
Matrix viewport(int x, int y, int w, int h, int depth)
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

Matrix zoom(float factor)
{
    Matrix Z = Matrix::identity(4);
    Z[0][0] = Z[1][1] = Z[2][2] = factor;
    return Z;
}

Matrix translation(Vec3f v)
{
    Matrix Tr = Matrix::identity(4);
    Tr[0][3] = v.x;
    Tr[1][3] = v.y;
    Tr[2][3] = v.z;
    return Tr;
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