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

const TGA_Color white = TGA_Color(255, 255, 255, 255);
const TGA_Color red = TGA_Color(255, 0, 0, 255);

void lessons_1(TGA_Image &image, int x, int y)
{
    draw_point(image, x, y, red);
}

void lessons_2(TGA_Image &image)
{
    draw_line(13, 20, 80, 40, image, white);
    draw_line(20, 13, 40, 80, image, red);
    draw_line(80, 40, 13, 20, image, red);
}

void lessons_3(Model *model, TGA_Image &image, int width, int height)
{
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
            draw_line(x0, y0, x1, y1, image, white);
        }
    }
}