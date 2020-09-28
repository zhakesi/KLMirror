#pragma once
#include <string>
#include "KLImage.h"
#include "KL3D.h"
#include "KLTexture.h"

class KLMedia {
public:
    static void InitMediaInfo(int mode, std::string &media);
    static KLImage *TriggerFrame();
    static GLuint ConvertFrameTexture(KLImage *frame);
    static KLTexture *GetResultTexture();
    static void FreeFrame();
    static void ReleaseMedia();
    static long GetDurationTimeMs();
    static long GetPresentTimeMs();
private:
    static int _mode;
    static std::string _media;
    static KLImage *_frame;
    static KLTexture *_texRE;
};