#include "../../include/KLColorMapFilter.h"
#include "../../include/GLUtil.h"
#include "../../include/KLImage.h"
#include "../../include/KLShaderFactory.h"

KLTexture* KLColorMapFilter::_lut_tex = nullptr;

void KLColorMapFilter::Init()
{
    KLImage image;
    image.load_from_file("lut.png");
    _lut_tex = new KLTexture;
    _lut_tex->updateTexIMG(&image);
    image.clear();
}

void KLColorMapFilter::Process(GLuint srcTex)
{
    if (!_lut_tex) {
        Init();
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, srcTex);
    GLuint lutTex = _lut_tex->getTexID();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, lutTex);

    auto shader = KLShaderFactory::GetCoreShader(KLE_SHADER_LUT_FILTER);
    GLuint prog = shader->program();
    glUseProgram(prog);
    glUniform1i(glGetUniformLocation(prog, "u_texture0"), 0);
    glUniform1i(glGetUniformLocation(prog, "u_texture_lut"), 1);

    glm::mat4 matrix = glm::mat4(1.0f);

    matrix = glm::translate(matrix, glm::vec3(-1.0f, -1.0f, 0.0f));
    matrix = glm::scale(matrix, glm::vec3(2.0f, 2.0f, 1.0f));
    glUniformMatrix4fv( glGetUniformLocation(prog, "u_2DMatrix"), 1, GL_FALSE,
                        glm::value_ptr(matrix));
}