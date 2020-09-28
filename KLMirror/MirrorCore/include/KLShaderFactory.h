#pragma once
#include <memory>

#include "KLShader.h"

class KLShaderFactory {
private:
    KLShaderFactory(){};
public:
    static std::shared_ptr<KLShader> GetCoreShader(KLE_SHADER_TYPE type);
};
