#ifndef __OUR_GL_H__
#define __OUR_GL_H__

#include "commons/tga_image.h"
#include "commons/matrix.h"

// extern Matrix ModelView;
// extern Matrix Viewport;
// extern Matrix Projection;

// void viewport(int x, int y, int w, int h);
// void projection(float coeff=0.f); // coeff = -1/c
// void lookat(Vec3f eye, Vec3f center, Vec3f up);

struct IShader {
    virtual ~IShader();
    virtual Vec4f vertex(int iface, int nthvert, Vec3f light_dir) = 0;
    virtual bool fragment(Vec3f bar, TGA_Color &color) = 0;
};

// void triangle(Vec4f *pts, IShader &shader, TGA_Image &image, TGA_Image &zbuffer);

#endif //__OUR_GL_H__

