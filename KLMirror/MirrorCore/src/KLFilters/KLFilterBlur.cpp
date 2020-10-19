#include "../../include/KLFilters/KLFilterBlur.h"
#include "../../include/KLShaderFactory.h"
#include "../../include/GLUtil.h"

KLFilterBlur::KLFilterBlur()
{
    _texHalf = nullptr;
    _depth = 0;
    _fbo = 0;
    _vbo = 0;
}

KLFilterBlur::~KLFilterBlur()
{

}

void KLFilterBlur::Init(int w, int h)
{
    _texHalf = new KLTexture(w, h, KLE_TEX_RGBA);

    GLUtil::CreateDepthbuffer(_depth, w, h);

    GLUtil::CreateFramebuffer(_fbo, _texHalf->getTexID(), _depth);

    GLUtil::CreatePanelVBO(_vbo);
}

void KLFilterBlur::GenerateGuassianHoriz(KLTexture *srcTex)
{
    int srcWidth = srcTex->w();
    int srcHeight = srcTex->h();

    GLint pre_fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &pre_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, srcWidth, srcHeight);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, srcTex->getTexID());

    auto shader = KLShaderFactory::GetCoreShader(KLE_SHADER_GUASS1D_FILTER);
    GLuint prog = shader->program();
    glUseProgram(prog);
    glUniform1i(glGetUniformLocation(prog, "u_texSRC"), 0);
    float offset1[] = {1.3846f/srcWidth, 0.0};
    float offset2[] = {3.2308f/srcWidth, 0.0};
    glUniform2fv(glGetUniformLocation(prog, "offset1"), 1, offset1);
    glUniform2fv(glGetUniformLocation(prog, "offset2"), 1, offset2);

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

    glBindFramebuffer(GL_FRAMEBUFFER, pre_fbo);
}

void KLFilterBlur::GuassianUSM(KLTexture * srcTex, KLTexture *blurTex)
{
    GLuint blurID = blurTex->getTexID();
    GLuint srcID = srcTex->getTexID();
    int texWidth = blurTex->w();
    int texHeight = blurTex->h();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, srcID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, blurID);

    auto shader = KLShaderFactory::GetCoreShader(KLE_SHADER_BLUR_FILTER);
    GLuint prog = shader->program();
    glUseProgram(prog);
    glUniform1i(glGetUniformLocation(prog, "u_texSRC"), 0);
    glUniform1i(glGetUniformLocation(prog, "u_texHoriz"), 1);
    float offset1[] = {0.0f, 1.3846f/ texHeight};
    float offset2[] = {0.0f, 3.2308f/ texHeight};
    glUniform2fv(glGetUniformLocation(prog, "offset1"), 1, offset1);
    glUniform2fv(glGetUniformLocation(prog, "offset2"), 1, offset2);

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

void KLFilterBlur::Process(KLTexture *srcTex)
{
    int srcWidth = srcTex->w();
    int srcHeight = srcTex->h();

    if (_texHalf == nullptr) {
        Init(srcWidth, srcHeight);
    }
    int range[4];
    glGetIntegerv(GL_VIEWPORT, range);
    GenerateGuassianHoriz(srcTex);
    int viewWidth = range[2];
    int viewHeight = range[3];
    glViewport(0, 0, viewWidth, viewHeight);
    GuassianUSM(srcTex, _texHalf);
}