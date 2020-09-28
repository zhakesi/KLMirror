#pragma once

#include "KL3D.h"
#include "KLTexture.h"

class KLColorMapFilter {
public:
    static void Init();
    static void Process(GLuint srcTex);
private:
    static KLTexture *_lut_tex;
};