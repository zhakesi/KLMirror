#pragma once

#include "../KL3D.h"
#include "../KLTexture.h"

class KLFilterFastGuassian {
public:
    KLFilterFastGuassian();
    ~KLFilterFastGuassian();
    void Process(KLTexture *srcTex);

private:
    void Init(int w, int h);
    void GenerateGuassianHoriz(KLTexture *srcTex);
    void GuassianUSM(KLTexture *srcTex, KLTexture *blurTex);
private:
    KLTexture *_texHalf;
    GLuint _depth;
    GLuint _fbo;
    GLuint _vbo;
};