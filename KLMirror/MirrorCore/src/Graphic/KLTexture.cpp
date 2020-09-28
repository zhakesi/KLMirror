#include "../../include/KLTexture.h"
#include "../../include/AndroidLog.h"
#include "../../include//GLUtil.h"

KLTexture::KLTexture(KLE_TEX_TYPE type)
{
    _w = 0;
    _h = 0;
    _type = type;
    GLUtil::CreateTexture(_texID);
}
KLTexture::KLTexture( int w, int h, KLE_TEX_TYPE type)
{
    _w = w;
    _h = h;
    _type = type;
    GLUtil::CreateTexture(_texID, w, h, type, nullptr);
}

KLTexture::~KLTexture()
{
    glDeleteTextures(1, &_texID);
    _texID = 0;
}

void KLTexture::updateTexIMG(KLImage *image)
{
    _w = image->w();
    _h = image->h();
    glBindTexture(GL_TEXTURE_2D, _texID);
    switch (image->format()) {
        case KLE_PIX_FMT_RGB888:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _w, _h,
                         0, GL_RGB, GL_UNSIGNED_BYTE, image->data());
            break;
        case KLE_PIX_FMT_RGBA888:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _w, _h,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, image->data());
            break;
        default:
            KLLOG_E("Texture Not Support Type");
            assert(0);
            break;
    }
}

void KLTexture::updateTexIMG(int w, int h, void *data)
{
    _w = w;
    _h = h;
    glBindTexture(GL_TEXTURE_2D, _texID);
    switch (_type) {
        case KLE_TEX_RGBA:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _w, _h,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        case KLE_TEX_LUM:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, _w, _h,
                         0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
            break;
        case KLE_TEX_UV:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _w/4, _h/2,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        default:
            KLLOG_E("Texture Not Support Type");
            assert(0);
            break;
    }
}

GLuint KLTexture::getTexID()
{
    return _texID;
}

int KLTexture::w()
{
    return _w;
}

int KLTexture::h()
{
    return _h;
}