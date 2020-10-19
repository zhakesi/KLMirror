#include "../../include/KLShaderFactory.h"
#include "../../include/AndroidLog.h"
#include "../../include/KLFile.h"
#include <string>
#include <map>
using namespace std;

static std::shared_ptr<KLShader> core_shares[KLE_SHADER_NR] = {};

std::shared_ptr<KLShader> KLShaderFactory::GetCoreShader(KLE_SHADER_TYPE type)
{
    shared_ptr<KLShader> ptr = nullptr;
    static map<KLE_SHADER_TYPE, string> fileMap = {
            {KLE_SHADER_TEXTURE, "shaders/texture.glsl"},
            {KLE_SHADER_YUVTEXTURE, "shaders/yuv_texture.glsl"},
            {KLE_SHADER_GRAY_FILTER, "shaders/filter_gray.glsl"},
            {KLE_SHADER_MEDIAN_FILTER, "shaders/filter_median.glsl"},
            {KLE_SHADER_LUT_FILTER, "shaders/filter_color_map.glsl"},
            {KLE_SHADER_GUASS1D_FILTER, "shaders/filter_guass1d.glsl"},
            {KLE_SHADER_BLUR_FILTER, "shaders/filter_blur.glsl"},
            {KLE_SHADER_4K_LUM_FILTER, "shaders/filter_4k_lum.glsl"},
            {KLE_SHADER_4K_LIGHTEST_FILTER, "shaders/filter_4k_lighest.glsl"},
            {KLE_SHADER_4K_GRADIENT_FILTER, "shaders/filter_4k_gradient.glsl"},
            {KLE_SHADER_4K_PUSHGRAD_FILTER, "shaders/filter_4k_pushgrad.glsl"},
    };

    if (!core_shares[type]) {
        core_shares[type] = make_shared<KLShader>(type);
        auto ret = core_shares[type]->createProgramFromFile(fileMap[type].c_str());
        assert(ret);
    }
    ptr = core_shares[type];
    return ptr;
}

