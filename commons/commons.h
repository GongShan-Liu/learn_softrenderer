#ifndef __COMMONS_H__
#define __COMMONS_H__

// 从3D转2D
Vec3f m2v(Matrix m);

// 从2D转3D
Matrix v2m(Vec3f v);

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
Matrix viewport(int x, int y, int w, int h, int depth);

Matrix zoom(float factor);

Matrix translation(Vec3f v);

Matrix rotation_x(float cosangle, float sinangle);

Matrix rotation_y(float cosangle, float sinangle);

Matrix rotation_z(float cosangle, float sinangle);

/* 相机矩阵 */
Matrix lookat(Vec3f eye, Vec3f center, Vec3f up);

/* 投影 (透视) 矩阵*/
Matrix projection(float coeff);

#endif //__MATRIX_H__