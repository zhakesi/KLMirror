#pragma once
#include "../KL3D.h"
#include "GLUtil.h"

class KLFilterNULL {
public:
    KLFilterNULL();
    ~KLFilterNULL();
    void Init(int w, int h);
    void Process(GLuint srcTex);

private:
    GLFBO_DAT _fb_dat;
    GLuint _vbo;
};