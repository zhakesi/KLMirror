#pragma once

#include <string>
#include "KLImage.h"

class KLPictureMedia {
public:
    static bool InitMediaInfo(std::string &media);
    static KLImage *GetFrame();
    static void ReleaseMedia();
private:
    static KLImage *_image;
};