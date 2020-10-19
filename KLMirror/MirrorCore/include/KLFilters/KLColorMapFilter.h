#pragma once

#include "KL3D.h"
#include "KLTexture.h"

class KLColorMapFilter {

public:
    KLColorMapFilter();
    void Init();
    void Process(GLuint srcTex);
private:
    KLTexture *_lut_tex;
    GLuint _vbo = 0;
};