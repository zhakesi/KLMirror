#pragma once
#include "KL3D.h"
#include "KLImage.h"

enum KLE_TEX_TYPE {
    KLE_TEX_RGBA,
    KLE_TEX_LUM,
    KLE_TEX_UV,
};

class KLTexture {
public:
    KLTexture(KLE_TEX_TYPE type = KLE_TEX_RGBA);
    KLTexture(int w, int h, KLE_TEX_TYPE type = KLE_TEX_RGBA);
    ~KLTexture();
    void updateTexIMG(KLImage *image);
    void updateTexIMG(int w, int h, void *data);
    GLuint getTexID();
    int w();
    int h();

private:
    int _w;
    int _h;
    KLE_TEX_TYPE _type;
    GLuint _texID;
};