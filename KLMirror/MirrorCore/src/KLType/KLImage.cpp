#include <map>
#include "../../include/KLImage.h"
#include "../../include/KLFile.h"
#include "../../include/AndroidLog.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../include/stb/stb_image_write.h"

#include <fstream>

static inline int _kl_buffer_size(int w, int h, KLE_PIXEL_FORMAT fmt)
{
    int buf_size = 0;
    switch (fmt) {
        case KLE_PIX_FMT_RGB888:
            buf_size = w * h * 3;
            break;
        case KLE_PIX_FMT_RGBA888:
            buf_size = w * h * 4;
            break;
        case KLE_PIX_FMT_YUV420:
            buf_size = w * h * 3 / 2;
            break;
        default:
            break;
    }
    return buf_size;
}

KLImage::KLImage()
{
    _w = 0, _h = 0;
    _fmt = KLE_PIX_FMT_INVALID;
    _buf_size = 0;
    _pixel = nullptr;
}

KLImage::KLImage(unsigned w, unsigned h, KLE_PIXEL_FORMAT fmt, bool create_buf)
{
    _w = w, _h = h;
    _fmt = fmt;
    _buf_size = _kl_buffer_size(w, h, fmt);
    if (create_buf) {
        _pixel = new uint8_t [_buf_size];
    }
}

KLImage::~KLImage()
{

}

bool KLImage::load_from_file(const char *path)
{
    bool ret = true;
    int x, y, comp;
    KLFileIStream in_stream;
    int file_size = 0;
    ret = in_stream.open(path, file_size);
    if (!ret) {
        KLLOG_E("load_image failed:%s\n", path);
        return ret;
    }

    void *raw_file = new unsigned char[file_size];
    int read_size = in_stream.read(raw_file, file_size);
    assert(read_size == file_size);

    auto pixel_dat = stbi_load_from_memory((stbi_uc const *)raw_file,
                                           file_size, &x, &y, &comp, 4);

    _w = x; _h = y;
    _fmt = KLE_PIX_FMT_RGBA888;

    _buf_size = _kl_buffer_size(_w, _h, _fmt);
    _pixel = (void *)pixel_dat;

    KLLOG_I("load image info:%d x %d channel %d fmt %d\n",
            x, y, comp, _fmt);
    return ret;
}

bool KLImage::save_to_file(const char *path)
{
    using namespace std;
    int channel = 4;
    if (_fmt == KLE_PIX_FMT_RGB888)
        channel = 3;
    auto ret = stbi_write_bmp(path,
                                   _w, _h, channel, _pixel);
    if (ret == 0) {
        KLLOG_I("save picture failed\n");
        return false;
    }
    return true;
}

KLE_PIXEL_FORMAT KLImage::format()
{
    return _fmt;
}

unsigned KLImage::w()
{
    return _w;
}

unsigned KLImage::h()
{
    return  _h;
}

void* KLImage::data()
{
    return _pixel;
}

void KLImage::set_data(void *buf)
{
    _pixel = buf;
}

unsigned KLImage::buffer_size()
{
    return _buf_size;
}

void KLImage::clear()
{
    _w = 0, _h = 0;
    _fmt = KLE_PIX_FMT_INVALID;
    _buf_size = 0;

    if (_pixel)
        delete[] (uint8_t *)_pixel;
    _pixel = nullptr;
}