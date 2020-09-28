#pragma once
#include <string>

enum KLE_PIXEL_FORMAT {
    KLE_PIX_FMT_INVALID = -1,
    KLE_PIX_FMT_RGB888 = 0,
    KLE_PIX_FMT_RGBA888,
    KLE_PIX_FMT_YUV420,
};

class KLImage {
public:
    KLImage();
    KLImage(unsigned w, unsigned h,
            KLE_PIXEL_FORMAT fmt, bool create_buf = false);
    bool load_from_file(const char *path);
    bool save_to_file(const char *path);
    ~KLImage();
    void clear();
    unsigned w();
    unsigned h();
    void *data();
    void set_data(void *buf);
    KLE_PIXEL_FORMAT format();
    unsigned buffer_size();
private:
    unsigned _w, _h;
    KLE_PIXEL_FORMAT _fmt;
    unsigned _buf_size;
    void *_pixel;
};