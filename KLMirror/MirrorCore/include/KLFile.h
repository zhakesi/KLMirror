#pragma once
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
class KLFileAsset{
public:
    static void init(AAssetManager *mgr);
    static AAssetManager *get_asset_manager();
};

class KLFileIStream {
public:
    KLFileIStream();
    ~KLFileIStream();
    bool open(const char *path, int & length);
    int read(void *buffer, long size);
    void close();

private:
    AAsset* _testAsset;
};