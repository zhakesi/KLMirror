#include "../../include/KLPictureMedia.h"
#include "../../include/AndroidLog.h"
#include <string>

KLImage *KLPictureMedia::_image = nullptr;

bool KLPictureMedia::InitMediaInfo(std::string &media)
{
    _image = new KLImage();
    KLLOG_I("xxx load image");
    _image->load_from_file(media.c_str());
    return true;
}

KLImage *KLPictureMedia::GetFrame()
{
    return _image;
}

void KLPictureMedia::ReleaseMedia()
{
    if (_image) {
        _image->clear();
        delete _image;
        _image = nullptr;
    }
}