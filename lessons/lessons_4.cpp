#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "commons/tga_image.h"
#include "commons/model.h"
#include "commons/our_gl.h"
#include "commons/matrix.h"
#include "commons/commons.h"
#include "commons/draw_point.h"
#include "commons/draw_line.h"
#include "commons/draw_triangle.h"
#include "lessons/lessons_4.h"

Vec4f GouraudShader::vertex(int iface, int nthvert, Vec3f light_dir)
{
    Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert));                               // read the vertex from .obj file
    gl_Vertex = Viewport * Projection * ModelView * gl_Vertex;                             // transform it to screen coordinates
    varying_intensity[nthvert] = std::max(0.f, model->normal(iface, nthvert) * light_dir); // get diffuse lighting intensity
    return gl_Vertex;
}

bool GouraudShader::fragment(Vec3f bar, TGA_Color &color)
{

    float intensity = varying_intensity * bar;    // interpolate intensity for the current pixel
    color = TGA_Color(255, 255, 255) * intensity; // well duh
    return false;                                 // no, we do not discard this pixel

    // float intensity = varying_intensity * bar;
    // if (intensity > .85)
    //     intensity = 1;
    // else if (intensity > .60)
    //     intensity = .80;
    // else if (intensity > .45)
    //     intensity = .60;
    // else if (intensity > .30)
    //     intensity = .45;
    // else if (intensity > .15)
    //     intensity = .30;
    // else
    //     intensity = 0;
    // color = TGA_Color(255, 155, 0) * intensity;
    // return false;
}

void lessons_10(Model *model, TGA_Image &image, int width, int height)
{
    const int depth = 255;

    Vec3f light_dir(1, 1, 1);
    Vec3f eye(1, 1, 3);
    Vec3f center(0, 0, 0);
    Vec3f up(0, 1, 0);

    Matrix lookat_matrix = lookat(eye, center, up);
    Matrix viewport_matrix = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
    Matrix projection_matrix = projection(-1.f / (eye - center).norm());
    light_dir.normalize();

    TGA_Image zbuffer(width, height, TGA_Image::GRAYSCALE);
    GouraudShader shader(model, lookat_matrix, viewport_matrix, projection_matrix);

    for (int i = 0; i < model->nfaces(); i++)
    {
        Vec4f screen_coords[3];
        for (int j = 0; j < 3; j++)
        {
            screen_coords[j] = shader.vertex(i, j, light_dir);
        }
        triangle(screen_coords, shader, image, zbuffer);
    }
}
