#pragma once

#include "KL3D.h"
#include "KLTexture.h"

enum KL_PLAY_STATUS {
    KL_PLAY_STATUS_PLAY,
    KL_PLAY_STATUS_PAUSE
};

class KLTexturePlayer {
public:
    KLTexturePlayer();
    ~KLTexturePlayer();
    void onResize(int w, int h);
    void renderTexture();
    void setPlayStatus(int status);
    void ApplayFilter(bool ret);

    static void updateSplit();
    static float SplitLine();
    static int W();
    static int H();
private:
    void createVBO();
private:
    static int _w;
    static int _h;
    static float _split;
    KLTexture *_texture;

    bool _use_filter;
    KL_PLAY_STATUS _play_status;
};