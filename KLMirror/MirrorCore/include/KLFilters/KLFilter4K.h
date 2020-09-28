#pragma once

#include "KL3D.h"
#include "GLUtil.h"
#include "KLTexture.h"

class KLFilter4K {
public:
    KLFilter4K();
    ~KLFilter4K();
    void Init(int w, int h);
    void Process(KLTexture* srcTex);
    void ComputeLum(GLuint srcTex);
    void PushLighest(GLuint srcTex);
    void ComputeGradient(GLuint srcTex);
    void PushGrad(GLuint srcTex);

private:
    GLFBO_DAT _fb_dat0;
    GLFBO_DAT _fb_dat1;
    GLFBO_DAT _fb_dat2;
    GLFBO_DAT _fb_dat3;
    GLuint  _vbo;
};