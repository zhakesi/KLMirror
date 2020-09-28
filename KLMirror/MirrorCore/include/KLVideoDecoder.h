#pragma once
#include <string>
#include <media/NdkMediaCodec.h>
#include <media/NdkMediaExtractor.h>
#include "KLImage.h"

enum KL_VIDEO_BUF_STATE {
    KL_VIDEO_BUF_INIT,
    KL_VIDEO_BUF_FREE,
    KL_VIDEO_BUF_READY,
};

class KLVideoDecoder {
public:
    static bool SetSource(std::string &video);
    static void StartDecode();
    static KLImage *TriggerFrame();
    static void FreeFrame();
    static void Destory();
    static long GetDurationTimeMs();
    static long GetPresentTimeMs();
private:
    static void ThreadLooper(void *data);
    static void SetBufferStatus(KL_VIDEO_BUF_STATE state);
    static int queueInputBuffer(AMediaCodec* codec, AMediaExtractor* extractor);
    static int dequeueOutputBuffer(AMediaCodec* codec, AMediaExtractor* extractor);
private:
    static AMediaExtractor* _extractor;
    static AMediaCodec* _codec;
    static KL_VIDEO_BUF_STATE _status;
    static KLImage *_frame;
};