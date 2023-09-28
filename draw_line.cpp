#include "stdio.h"

#include "tgaimage.h"
#include "draw_line.h"
#include "geometry.h"

/***
绘制直线算法，意思就是 在一个 n*m的像素(可理解为网格)中 找到 点A(x0, y0) 连接到 点B(x1, y1) 的最短距离的网格

    例如在 5*5 中找到 点A(0,0) 到 点B(4, 4) 的网格

      (4,0) #     #     #     B
      (3,0) #     #     #     #
      (2,0) #     #     #     #
      (1,0) #     #     #     #
        A (0,1) (0,2) (0,3) (0,4)
***/

/*** 该方法是把 点A到点B的做组成的 min(x0, x1) * max(y0, y1) 的网格进行迭代找出他们之间的最短网格
        该方法除了性能消耗较大同时也取决于 t的每次步长常数的选择，如果取它为0.1，那么如果点A(13, 20) 点B(80, 40)就出现连接不上的点
***/

// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
// {
//     for (float t = 0; t < 1.0; t += 0.01)
//     {
//         printf("t=%f", t);
//         int x = x0 + (x1 + x0) * t;
//         int y = y0 + (y1 - y0) * t;

//         printf(" x=%i", x);
//         printf(" y=%i", y);

//         image.set(x, y, color);

//         printf("\n");
//     }
// }

/*** 该方法（错误的）是以x轴+1为迭代的步长
        问题：
            1. 如果x1 > x0那么就无法迭代，正常来说可以对称迭代，如A-B, B-A
            2. 如果|x1 - y1| >= |x0 - y0|, 那么就会出现连接不上的点，正常是所有点都连接成直线
***/

// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
// {
//     for (int x = x0; x <= x1; x++)
//     {
//         float t = (x - x0) / (float)(x1 - x0);

//         printf("t=%f", t);

//         int y = y0 * (1.0 - t) + y1 * t;

//         printf(" x=%i", x);
//         printf(" y=%i", y);

//         image.set(x, y, color);

//         printf("\n");

//     }
//     printf("\n");
// }

/***  该方法就是通过交换的方式，让 x0 < x1 和 x0和x1的距离 > y0和y1的距离 , 这样就能以x轴+1为迭代的步长保持正确
            问题：如果绘制大量的line，那么该方法性能在 计算t和y值时，就会消耗较大 (因为存在浮点乘除的迭代计算)，所以可进一步优化
***/

// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
// {
//     bool steep = false;

//     // 如果 y 比 x 长，那么就转置变换xy
//     if (std::abs(x0 - x1) < std::abs(y0 - y1))
//     {
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//         steep = true;
//     }

//     // 如果x0 > x1, 那么就转置变换
//     if (x0 > x1)
//     {
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }

//     for (int x = x0; x <= x1; x++)
//     {
//         float t = (x - x0) / (float)(x1 - x0);
//         int y = y0 * (1.0 - t) + y1 * t;
//         if (steep)
//         {
//             // 有转置，那么就反转置
//             image.set(y, x, color);
//         }
//         else
//         {
//             image.set(x, y, color);
//         }
//     }
// }

/*** 该方法是通过 dy 与 dx 的比值得到一个误差值, 当累积误差大于0.5 (即大于1个像素), 即y要增(减)1并累积误差也减1
        好处是把计算t与y的浮点乘除优化了, 那么还可以进一步的优化
***/

// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
// {
//     bool steep = false;
//     if (std::abs(x0 - x1) < std::abs(y0 - y1))
//     {
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//         steep = true;
//     }

//     if (x0 > x1)
//     {
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }

//     int dx = x1 - x0;
//     int dy = y1 - y0;

//     // 误差（即dy与dx的比值）, 累积误差的步长
//     float derror = std::abs(dy / float(dx));

//     float error = 0;
//     int y = y0;

//     for (int x = x0; x <= x1; x++)
//     {
//         if (steep)
//         {
//             image.set(y, x, color);
//         }
//         else
//         {
//             image.set(x, y, color);
//         }

//         // 当累积误差大于0.5时 (即大于1个像素)，y 就增(减) 1，累积误差又退回原始累积
//         error += derror;
//         if (error > 0.5)
//         {
//             y += (y1 > y0 ? 1 : -1);
//             error -= 1.0;
//         }
//     }
// }

/*** 该方法是 累积误差 (2倍dy的叠加) 大于dx时，那么y就增(减)1, 并且 误差累积通过减去2倍的dx，退回到低于dx的累积误差
 *      目前可认为较良好的绘制直线的方案，也可称为 Bresenham直线算法
 ***/

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    // int dx = x1 - x0;
    // int dy = y1 - y0;

    // 2倍的dy作为累积误差的步长
    // int derror = std::abs(dy) * 2;

    // int error = 0;
    // int y = y0;

    // for (int x = x0; x <= x1; x++)
    // {
    //     if (steep)
    //     {
    //         image.set(y, x, color);
    //     }
    //     else
    //     {
    //         image.set(x, y, color);
    //     }

    //     // 当累积误差 (以2倍dy的步长进行叠加) 大于dx时，那么y就增(减)1, 并且 误差累积通过减去2倍的dx，退回到低于dx的累积误差
    //     error += derror;
    //     if (error > dx)
    //     {
    //         y += (y1 > y0 ? 1 : -1);
    //         error -= dx * 2;
    //     }
    // }

    // 良好的优化
    // if (steep)
    // {
    //     for (int x = x0; x <= x1; ++x)
    //     {
    //         image.set(y, x, color);
    //         error += derror;
    //         if (error > dx)
    //         {
    //             y += (y1 > y0 ? 1 : -1);
    //             error -= dx * 2;
    //         }
    //     }
    // }
    // else
    // {
    //     for (int x = x0; x <= x1; ++x)
    //     {
    //         image.set(x, y, color);
    //         error += derror;
    //         if (error > dx)
    //         {
    //             y += (y1 > y0 ? 1 : -1);
    //             error -= dx * 2;
    //         }
    //     }
    // }

    for (int x = x0; x <= x1; x++)
    {
        float t = (x - x0) / (float)(x1 - x0);
        int y = y0 * (1. - t) + y1 * t;
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
    }
}


void line(Vec3f p0, Vec3f p1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(p0.x-p1.x)<std::abs(p0.y-p1.y)) {
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
        steep = true;
    }
    if (p0.x>p1.x) {
        std::swap(p0, p1);
    }

    for (int x=p0.x; x<=p1.x; x++) {
        float t = (x-p0.x)/(float)(p1.x-p0.x);
        int y = p0.y*(1.-t) + p1.y*t+.5;
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}