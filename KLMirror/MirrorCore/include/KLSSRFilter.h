#pragma once

#include "KL3D.h"

class KLSSRFilter {
public:
    void process(GLuint src_tex);
    GLuint getResultText();
private:
    static GLuint _lum_tex;
};