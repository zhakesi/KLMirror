#include "../../include/KLVideoMedia.h"
#include "../../include/KLVideoDecoder.h"

KLImage *KLVideoMedia::_image = nullptr;

bool KLVideoMedia::InitMediaInfo(std::string &media)
{
    bool ret = KLVideoDecoder::SetSource(media);
    if (!ret) {
        assert(0);
    }
    KLVideoDecoder::StartDecode();
    return true;
}

KLImage *KLVideoMedia::GetFrame()
{
    return KLVideoDecoder::TriggerFrame();
}

void KLVideoMedia::FreeFrame()
{
    KLVideoDecoder::FreeFrame();
}

void KLVideoMedia::ReleaseMedia()
{
    if (_image) {
        _image->set_data(nullptr);
        delete _image;
        _image = nullptr;
    }
}

long KLVideoMedia::GetDurationTimeMs()
{
    return KLVideoDecoder::GetDurationTimeMs();
}

long KLVideoMedia::GetPresentTimeMs()
{
    return KLVideoDecoder::GetPresentTimeMs();
}