//
// Created by liqiao on 2020/6/16.
//
#pragma once

#include <android/log.h>
#define  INFO_TAG    "klmirror_info"
#define  ERROR_TAG    "klmirror_error"
#define  KLLOG_I(...)  __android_log_print(ANDROID_LOG_INFO,INFO_TAG,__VA_ARGS__)
#define  KLLOG_E(...)  __android_log_print(ANDROID_LOG_ERROR,ERROR_TAG,__VA_ARGS__)
