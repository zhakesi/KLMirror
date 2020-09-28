#pragma once
#include "KLImage.h"
#include "KLTexture.h"
#include "KL3D.h"

class YUVConvert {
public:
    static void ConvertTexture(KLImage *image, KLTexture *texture);

private:
    static void Initialize(int w, int h, GLuint _color);
    static KLTexture *_texY;
    static KLTexture *_texUV;
    static GLuint _depth;
    static GLuint _fbo;
    static GLuint _vbo;
};