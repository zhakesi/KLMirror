#include "../../include/KLMedia.h"
#include "../../include/AndroidLog.h"
#include "../../include/KLPictureMedia.h"
#include "../../include/KLVideoMedia.h"
#include "../../include/YUVConvert.h"

int KLMedia::_mode = 0;
std::string KLMedia::_media = "";
KLImage *KLMedia::_frame = nullptr;
KLTexture *KLMedia::_texRE = nullptr;

enum {
    VIDEO_MODE = 0,
    PICTURE_MODE = 1,
};

void KLMedia::InitMediaInfo(int mode, std::string &media)
{
    _mode = mode;
    _media = media;
    if (_mode == PICTURE_MODE) {
        KLPictureMedia::InitMediaInfo(media);
    } else if (_mode == VIDEO_MODE) {
        KLVideoMedia::InitMediaInfo(media);
    } else {
        KLLOG_E("media stream type not support!\n");
        assert(0);
    }
}

KLImage *KLMedia::TriggerFrame()
{
    if (_mode == PICTURE_MODE) {
        _frame = KLPictureMedia::GetFrame();
    } else {
        _frame = KLVideoMedia::GetFrame();
    }
    return _frame;
}

void KLMedia::FreeFrame()
{
    if (_mode == VIDEO_MODE) {
        KLVideoMedia::FreeFrame();
    }
}

GLuint KLMedia::ConvertFrameTexture(KLImage *frame)
{
    auto fmt = frame->format();
    if (_texRE == nullptr) {
        _texRE = new KLTexture(frame->w(), frame->h(), KLE_TEX_RGBA);
    }
    if ( fmt == KLE_PIX_FMT_RGB888 ||
        frame->format() == KLE_PIX_FMT_RGBA888) {
        _texRE->updateTexIMG(frame);
    } else if (fmt == KLE_PIX_FMT_YUV420) {
        YUVConvert::ConvertTexture(_frame, _texRE);
    } else {
        KLLOG_E("Unknown Pixel Format!!!");
        assert(0);
    }
    return _texRE->getTexID();
}

KLTexture *KLMedia::GetResultTexture()
{
    return _texRE;
}

void KLMedia::ReleaseMedia()
{
    if (_mode == VIDEO_MODE) {
        KLVideoMedia::ReleaseMedia();
    } else {
        KLPictureMedia::ReleaseMedia();
    }
    _frame = nullptr;
}

long KLMedia::GetDurationTimeMs()
{
    if (_mode == VIDEO_MODE) {
        return KLVideoMedia::GetDurationTimeMs();
    } else {
        return 1;
    }
}

long KLMedia::GetPresentTimeMs()
{
    if (_mode == VIDEO_MODE) {
        return KLVideoMedia::GetPresentTimeMs();
    } else {
        return 1;
    }
}