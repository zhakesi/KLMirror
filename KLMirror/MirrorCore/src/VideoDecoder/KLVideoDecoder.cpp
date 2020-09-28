#include "../../include/KLVideoDecoder.h"

#include "../../include/AndroidLog.h"
#include "../../include/KLFile.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <thread>
#include <mutex>
#include <condition_variable>

AMediaExtractor* KLVideoDecoder::_extractor = nullptr;
AMediaCodec* KLVideoDecoder::_codec = nullptr;
KLImage *KLVideoDecoder::_frame = nullptr;
KL_VIDEO_BUF_STATE KLVideoDecoder::_status = KL_VIDEO_BUF_INIT;
static std::mutex mtx;

static int64_t durationTimeUs = 0;

static int64_t presentationTimeUs;

bool KLVideoDecoder::SetSource(std::string &video)
{
    off64_t outStart, outLen;
    AAssetManager *asset_mgr = KLFileAsset::get_asset_manager();
    AAsset* asset = AAssetManager_open(asset_mgr, video.c_str(), 0);
    auto fd = AAsset_openFileDescriptor64(asset, &outStart, &outLen);
    if (fd < 0) {
        KLLOG_E("Can't Access File:%s\n", video.c_str());
        assert(0);
    }
    _extractor = AMediaExtractor_new();

    media_status_t err = AMediaExtractor_setDataSourceFd(_extractor, fd,
                                                         outStart,
                                                         outLen);
    close(fd);
    if (err != AMEDIA_OK) {
        KLLOG_E("setDataSource error: %d", err);
        return false;
    }
    int numtracks = AMediaExtractor_getTrackCount(_extractor);
    //KLLOG_I("input has %d tracks", numtracks);
    int32_t width = 0, height = 0;
    int64_t duration = 0;
    for (int i = 0; i < numtracks; i++) {
        AMediaFormat *format = AMediaExtractor_getTrackFormat(_extractor, i);
        // const char *s = AMediaFormat_toString(format);
        //KLLOG_I("track %d format: %s", i, s);
        const char *mime;
        if (!AMediaFormat_getString(format, AMEDIAFORMAT_KEY_MIME, &mime)) {
            KLLOG_E("no mime type");
            return false;
        }
        if (strncmp(mime, "video/", 6)) { //not video
            AMediaFormat_delete(format);
            continue;
        }
        AMediaExtractor_selectTrack(_extractor, i);
        _codec = AMediaCodec_createDecoderByType(mime);
        AMediaCodec_configure(_codec, format, NULL, NULL, 0);
        AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_WIDTH, &width);
        AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_HEIGHT, &height);
        AMediaFormat_getInt64(format, AMEDIAFORMAT_KEY_DURATION, &durationTimeUs);
        KLLOG_I("video resolution %d x %d, duration:%ds\n", width, height, durationTimeUs/1000000);
        AMediaCodec_start(_codec);
        AMediaFormat_delete(format);
        break;
    }
    if (width % 16) {
        width = ((int)(width / 16) + 1) * 16;
    }
    if (height % 16) {
        height = ((int)(height / 16) + 1) * 16;
    }
    _frame = new KLImage(width, height, KLE_PIX_FMT_YUV420, false);
    KLLOG_I("Media Codec Init!\n");
    return true;
}

void KLVideoDecoder::SetBufferStatus(KL_VIDEO_BUF_STATE state)
{
    std::lock_guard<std::mutex> lck (mtx);
    _status = state;
}

int KLVideoDecoder::queueInputBuffer(AMediaCodec* codec, AMediaExtractor* extractor)
{
    int ret = 1;
    ssize_t bufidx = AMediaCodec_dequeueInputBuffer(codec, 2000);
    if (bufidx < 0) {
        ret = -1;
        return ret;
    }
    size_t bufsize;
    auto buf = AMediaCodec_getInputBuffer(codec, bufidx, &bufsize);
    auto sampleSize = AMediaExtractor_readSampleData(extractor, buf, bufsize);

    if (sampleSize < 0) {
        KLLOG_E("End Of Input Stream");
        ret = 0;
    }
    presentationTimeUs = AMediaExtractor_getSampleTime(extractor);
    if (presentationTimeUs > durationTimeUs) {
        presentationTimeUs = durationTimeUs;
        return -1;
    }
    //KLLOG_I("presentationTimeUs :%d", presentationTimeUs);
    AMediaCodec_queueInputBuffer(codec, bufidx, 0, sampleSize, presentationTimeUs,
                                 ret == 0 ?  AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM : 0);
    AMediaExtractor_advance(extractor);
    return ret;
}

uint64_t systemmicrotime()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    uint64_t current = time.tv_sec * 1000 * 1000 + time.tv_usec;
    return current;
}

int KLVideoDecoder::dequeueOutputBuffer(AMediaCodec* codec, AMediaExtractor* extractor)
{
    int ret = 1;
    AMediaCodecBufferInfo info;
    auto index = AMediaCodec_dequeueOutputBuffer(codec, &info, 0);
    //KLLOG_I("Info flag:%d, size:%d, offset:%d\n", info.flags, info.size, info.offset);
    if (index < 0) {
        KLLOG_E("dequeueInputBuffer Failed!");
        ret = -1;
        return ret;
    }
    if (info.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM) {
        KLLOG_E("End Of Output Stream");
        ret = 0;
    }
    static uint64_t renderstart = systemmicrotime();
    int64_t delay = renderstart + presentationTimeUs - systemmicrotime();
//    KLLOG_I("delay time:%d", delay);
//    if (delay > 0) {
//        usleep(delay);
//    }
    //KLLOG_I("presentationTimeUs %u", presentationTimeUs);
    size_t bufsize;
    auto buffer = AMediaCodec_getOutputBuffer(codec, index, &bufsize);
//    KLLOG_I("buffer id  = %d", index);
//    KLLOG_I("decoder out buffer size:%d", bufsize);
    _frame->set_data(buffer);
    SetBufferStatus(KL_VIDEO_BUF_READY);
    while (_status != KL_VIDEO_BUF_FREE) {
        usleep(1000);
    }
    AMediaCodec_releaseOutputBuffer(codec, index, false);
    return ret;
}

void KLVideoDecoder::ThreadLooper(void *data)
{
    bool input_end = false, output_end = false;

    while (true) {
        if (input_end && output_end) {
            KLLOG_E("End Of All Stream\n");
            break;
        }

        if (!input_end) {
            auto ret = queueInputBuffer(_codec, _extractor);
            if (ret == 0)
                input_end = true;
        }

        if (!output_end) {
            auto ret = dequeueOutputBuffer(_codec, _extractor);
            if (ret == 0)
                output_end = true;
        }
    }
}

void KLVideoDecoder::StartDecode()
{
    std::thread _work_thread(ThreadLooper, nullptr);
    _work_thread.detach();
}

KLImage *KLVideoDecoder::TriggerFrame()
{
    while (_status != KL_VIDEO_BUF_READY) {
        usleep(1000);
    }
    return _frame;
}

void KLVideoDecoder::FreeFrame()
{
    SetBufferStatus(KL_VIDEO_BUF_FREE);
}

void KLVideoDecoder::Destory()
{

}

long KLVideoDecoder::GetDurationTimeMs()
{
    return durationTimeUs / 1000;
}

long KLVideoDecoder::GetPresentTimeMs()
{
    return presentationTimeUs / 1000;
}

