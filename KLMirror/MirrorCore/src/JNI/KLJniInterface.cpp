//
// Created by liqiao on 2020/6/16.
//

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "../../include/KLFile.h"
#include "../../include/KLMedia.h"
#include "../../include/KLShaderFactory.h"
#include "../../include//KLTexturePlayer.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
static KLTexturePlayer *player = nullptr;

static void graphicInit()
{
    player = new KLTexturePlayer();
}

static void onResize(int w, int h)
{
    player->onResize(w, h);
}

static void renderFrame()
{
    player->renderTexture();
}

static void setFilterOnOff(bool isOpen)
{
    player->ApplayFilter(isOpen);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" {
JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_graphicInit(JNIEnv *env,
        jclass clazz);

JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_onResize(JNIEnv *env,
        jclass clazz, jint width, jint height);

JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_renderFrame(JNIEnv *env,
        jclass clazz);

JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_setPlayStatus(JNIEnv *env,
        jclass clazz, jint status);

JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_initAssetMGR(JNIEnv * env,
        jclass obj, jobject assetManager);

JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_configMediaInfo(JNIEnv *env,
        jclass clazz, jint mode, jstring media);

JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_controlSwith(JNIEnv *env,
        jclass clazz, jboolean isOpen);

JNIEXPORT long JNICALL Java_com_klvideoplayer_KLJniInterface_getDurationTimeMs(JNIEnv *env,
        jclass clazz);
JNIEXPORT long JNICALL Java_com_klvideoplayer_KLJniInterface_getPresentTimeMs(JNIEnv *env,
        jclass clazz);

};

JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_graphicInit(JNIEnv *env,
    jclass clazz)
{
    graphicInit();
}

JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_onResize(JNIEnv *env,
    jclass clazz, jint width, jint height)
{
    onResize(width, height);
}

JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_renderFrame(JNIEnv *env,
    jclass clazz)
{
    renderFrame();
}

JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_setPlayStatus(JNIEnv *env,
    jclass clazz, jint status)
{
    player->setPlayStatus(status);
}

JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_initAssetMGR(JNIEnv * env,
    jclass obj, jobject assetManager)
{
    AAssetManager *nativeasset = AAssetManager_fromJava(env, assetManager);
    KLFileAsset::init(nativeasset);
}

JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_configMediaInfo(JNIEnv *env,
    jclass clazz, jint mode, jstring media)
{
    int media_type = mode;
    const char* media_name = env->GetStringUTFChars(media, 0);
    std::string str_media(media_name);
    env->ReleaseStringUTFChars(media, media_name);
    KLMedia::InitMediaInfo(media_type, str_media);
}

JNIEXPORT void JNICALL Java_com_klvideoplayer_KLJniInterface_controlSwith(JNIEnv *env,
        jclass clazz, jboolean isOpen)
{
    setFilterOnOff(isOpen);
}

JNIEXPORT long JNICALL Java_com_klvideoplayer_KLJniInterface_getDurationTimeMs(JNIEnv *env,
        jclass clazz)
{
    return KLMedia::GetDurationTimeMs();
}

JNIEXPORT long JNICALL Java_com_klvideoplayer_KLJniInterface_getPresentTimeMs(JNIEnv *env,
        jclass clazz)
{
    return KLMedia::GetPresentTimeMs();
}