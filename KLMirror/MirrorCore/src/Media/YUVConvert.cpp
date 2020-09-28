#include "../../include/YUVConvert.h"
#include "../../include/GLUtil.h"
#include "../../include/KLShaderFactory.h"
#include "../../include/AndroidLog.h"

KLTexture *YUVConvert::_texY = nullptr;
KLTexture *YUVConvert::_texUV = nullptr;
GLuint YUVConvert::_depth = 0;
GLuint YUVConvert::_fbo = 0;
GLuint YUVConvert::_vbo = 0;

void YUVConvert::ConvertTexture(KLImage *image, KLTexture *texture)
{
    int w = image->w();
    int h = image->h();
    GLuint colorTex = texture->getTexID();
    if (_texY == nullptr) {
        Initialize(w, h, colorTex);
    }
    size_t offset = w * h;
    _texY->updateTexIMG(w, h, image->data());
    uint8_t *uv = (uint8_t *)image->data() + offset;
    _texUV->updateTexIMG(w, h, uv);

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, w, h);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texY->getTexID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _texUV->getTexID());

    auto shader = KLShaderFactory::GetCoreShader(KLE_SHADER_YUVTEXTURE);
    GLuint prog = shader->program();
    glUseProgram(prog);
    glUniform1i(glGetUniformLocation(prog, "u_texY"), 0);
    glUniform1i(glGetUniformLocation(prog, "u_texVU"), 1);

    glm::mat4 matrix = glm::mat4(1.0f);

    matrix = glm::translate(matrix, glm::vec3(-1.0f, -1.0f, 0.0f));
    matrix = glm::scale(matrix, glm::vec3(2.0f, 2.0f, 1.0f));
    glUniformMatrix4fv( glGetUniformLocation(prog, "u_2DMatrix"), 1, GL_FALSE,
                        glm::value_ptr(matrix));


    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void*) 0 );
    glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                           (const void*) ( 2 * sizeof(GLfloat) ) );
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void YUVConvert::Initialize(int w, int h, GLuint _color)
{
    _texY = new KLTexture(KLE_TEX_LUM);
    _texUV = new KLTexture(KLE_TEX_UV);

    GLUtil::CreateDepthbuffer(_depth, w, h);

    GLUtil::CreateFramebuffer(_fbo, _color, _depth);

    GLUtil::CreatePanelVBO(_vbo);
}