#pragma once
#include "KLImage.h"


class KLVideoMedia {
public:
    static bool InitMediaInfo(std::string &media);
    static KLImage *GetFrame();
    static void FreeFrame();
    static void ReleaseMedia();
    static long GetDurationTimeMs();
    static long GetPresentTimeMs();
private:
    static KLImage *_image;
};