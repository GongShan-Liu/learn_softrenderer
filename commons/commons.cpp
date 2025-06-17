#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>

#include "commons/matrix.h"
#include "commons/commons.h"

/*
列优先的矩阵
3D 矩阵
    | sx   0   0   tx  |
    |  0   sy  0   ty  |
    |  0   0  sz   tz  |
    |  0   0   0   1   |
*/

// 从3D转2D
Vec3f m2v(Matrix m)
{
    return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

// 从2D转3D
Matrix v2m(Vec3f v)
{
    Matrix m = Matrix::identity();
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}

/*
视口就是窗口中用来显示图形的一块矩形区域，它可以和窗口等大，也可以比窗口大或者小

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
    Matrix m = Matrix::identity();
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = depth / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = depth / 2.f;
    return m;
}

/* 焦距 (缩放矩阵) */
Matrix zoom(float factor)
{
    Matrix Z = Matrix::identity();
    Z[0][0] = Z[1][1] = Z[2][2] = factor;
    return Z;
}

/* 位移矩阵 */
Matrix translation(Vec3f v)
{
    Matrix Tr = Matrix::identity();
    Tr[0][3] = v.x;
    Tr[1][3] = v.y;
    Tr[2][3] = v.z;
    return Tr;
}

/* 旋转矩阵(x) */
Matrix rotation_x(float cos_angle, float sin_angle)
{
    Matrix R = Matrix::identity();
    R[1][1] = R[2][2] = cos_angle;
    R[1][2] = -sin_angle;
    R[2][1] = sin_angle;
    return R;
}

/* 旋转矩阵(y) */
Matrix rotation_y(float cos_angle, float sin_angle)
{
    Matrix R = Matrix::identity();
    R[0][0] = R[2][2] = cos_angle;
    R[0][2] = sin_angle;
    R[2][0] = -sin_angle;
    return R;
}

/* 旋转矩阵(z) */
Matrix rotation_z(float cos_angle, float sin_angle)
{
    Matrix R = Matrix::identity();

    R[0][0] = R[1][1] = cos_angle;
    R[0][1] = -sin_angle;
    R[1][0] = sin_angle;
    return R;
}

/* 相机矩阵 */
Matrix lookat(Vec3f eye, Vec3f center, Vec3f up)
{
    Vec3f z = (eye - center).normalize();
    Vec3f x = (up ^ z).normalize();
    Vec3f y = (z ^ x).normalize();

    Matrix res = Matrix::identity();
    for (int i = 0; i < 3; i++)
    {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[i][3] = -center[i];
    }

    return res;
}

Matrix projection(float coeff)
{
    Matrix Projection = Matrix::identity();
    Projection[3][2] = coeff;
    return Projection;
}
