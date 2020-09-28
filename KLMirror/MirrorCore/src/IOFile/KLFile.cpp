#include "../../include/KLFile.h"
#include "../../include/AndroidLog.h"

static AAssetManager *kl_asset_mgr;
void KLFileAsset::init(AAssetManager *mgr)
{
    kl_asset_mgr = mgr;
}

AAssetManager *KLFileAsset::get_asset_manager()
{
    return kl_asset_mgr;
}

KLFileIStream::KLFileIStream()
{
    _testAsset = nullptr;
}

KLFileIStream::~KLFileIStream()
{
    if (_testAsset)
        close();
}
bool KLFileIStream::open(const char *path, int &size)
{
    if (_testAsset)
        close();

    AAssetManager *mgr = KLFileAsset::get_asset_manager();
    assert(NULL != mgr);

    _testAsset = AAssetManager_open(mgr, path, AASSET_MODE_UNKNOWN);
    if (!_testAsset) {
        KLLOG_E("open file failed! path:%s\n", path);
        size = -1;
        return false;

    }
    size = AAsset_getLength(_testAsset);
    return true;
}

int KLFileIStream::read(void *buffer, long size) {
    assert(_testAsset);
    int count = AAsset_read(_testAsset, buffer, size);
    return count;
}

void KLFileIStream::close() {
    AAsset_close(_testAsset);
    _testAsset = nullptr;
}