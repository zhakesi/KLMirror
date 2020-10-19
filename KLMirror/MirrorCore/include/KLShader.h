#pragma once
#include "KL3D.h"
#include <string>

enum KLE_SHADER_TYPE {
    KLE_SHADER_TEXTURE = 0,
    KLE_SHADER_YUVTEXTURE,
    KLE_SHADER_GRAY_FILTER,
    KLE_SHADER_MEDIAN_FILTER,
    KLE_SHADER_LUT_FILTER,
    KLE_SHADER_GUASS1D_FILTER,
    KLE_SHADER_BLUR_FILTER,
    KLE_SHADER_4K_LUM_FILTER,
    KLE_SHADER_4K_LIGHTEST_FILTER,
    KLE_SHADER_4K_GRADIENT_FILTER,
    KLE_SHADER_4K_PUSHGRAD_FILTER,
    KLE_SHADER_NR
};

class KLShader {
public:
    KLShader(KLE_SHADER_TYPE type);
    bool createProgram(const char *str_vertex, const char *str_frag);
    bool createProgramFromFile(const char *path);
    GLuint program();
private:
    KLE_SHADER_TYPE _type;
    GLuint _program;
};