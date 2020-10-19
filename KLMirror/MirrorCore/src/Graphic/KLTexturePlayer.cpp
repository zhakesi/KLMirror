#include <unistd.h>
#include "../../include/KLTexturePlayer.h"
#include "../../include/KLMedia.h"
#include "../../include/KLShaderFactory.h"
#include "../../include/AndroidLog.h"
#include "../../include/KLFilters/KLFilterNULL.h"
#include "../../include/KLFilters/KLColorMapFilter.h"
#include "../../include/KLFilters/KLFilterUSM.h"
#include "../../include/KLFilters/KLFilter4K.h"

int KLTexturePlayer::_w = 0;
int KLTexturePlayer::_h = 0;



KLTexturePlayer::KLTexturePlayer()
{
    _use_filter = false;
    _play_status = KL_PLAY_STATUS_PLAY;
}

KLTexturePlayer::~KLTexturePlayer()
{

}

void KLTexturePlayer::onResize(int w, int h)
{
    _w = w, _h = h;
}

void KLTexturePlayer::renderTexture()
{
    static GLuint texID;
    static KLImage *frame;
    if (_play_status == KL_PLAY_STATUS_PLAY) {
        frame = KLMedia::TriggerFrame();
        texID = KLMedia::ConvertFrameTexture(frame);
        KLMedia::FreeFrame();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, _w, _h);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    KLTexturePlayer::updateSplit();
    if (_use_filter) {
        static KLFilter4K *filter = new KLFilter4K;
        auto srcTex = KLMedia::GetResultTexture();
        filter->Process(srcTex);
    } else {
        static KLFilterNULL *filterNull = new KLFilterNULL;
        filterNull->Process(texID);
    }
}

void KLTexturePlayer::setPlayStatus(int status)
{
    _play_status = static_cast<KL_PLAY_STATUS>(status);
}

void KLTexturePlayer::ApplayFilter(bool ret)
{
    _use_filter = ret;
}

int KLTexturePlayer::W()
{
    return _w;
}

int KLTexturePlayer::H()
{
    return _h;
}

float KLTexturePlayer::_split = 0.0f;
void KLTexturePlayer::updateSplit()
{
    static bool plus = true;
    if (_split > 1.0f) {
        _split = 1.0f;
        plus = false;
    } else if (_split < 0.0f) {
        _split = 0.0f;
        plus = true;
    }
    _split = 0.5f;
//    if (plus)
//        _split +=0.004f;
//    else
//        _split -=0.004f;
}

float KLTexturePlayer::SplitLine()
{
    return _split;
}