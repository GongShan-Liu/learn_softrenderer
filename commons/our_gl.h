#ifndef __OUR_GL_H__
#define __OUR_GL_H__

#include "commons/tga_image.h"
#include "commons/matrix.h"

struct IShader
{
    virtual ~IShader() {};
    virtual Vec4f vertex(int iface, int nthvert, Vec3f light_dir) = 0;
    virtual bool fragment(Vec3f bar, TGA_Color &color) = 0;
};

#endif //__OUR_GL_H__
