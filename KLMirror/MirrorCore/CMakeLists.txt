cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-exceptions -fno-unwind-tables")

include_directories("include/")

add_library(klmirror SHARED
        src/JNI/KLJniInterface.cpp
        src/IOFile/KLFile.cpp
        src/KLType/KLImage.cpp
        src/VideoDecoder/KLVideoDecoder.cpp
        src/Media/KLPictureMedia.cpp
        src/Media/KLVideoMedia.cpp
        src/Media/KLMedia.cpp
        src/Media/YUVConvert.cpp
        src/Graphic/KLShader.cpp
        src/Graphic/KLShaderFactory.cpp
        src/Graphic/KLTexture.cpp
        src/Graphic/KLTexturePlayer.cpp
        src/Graphic/GLUtil.cpp
        src/KLFilters//KLColorMapFilter.cpp
        src/KLFilters/KLFilterNULL.cpp
        src/KLFilters/KLFilterBlur.cpp
        src/KLFilters/KLFilter4K.cpp)

# add lib dependencies
target_link_libraries(
    klmirror
    android
    mediandk
    log
    EGL
    GLESv3)