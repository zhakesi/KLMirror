#include "KLFilters/KLFilter4K.h"
#include "KLShaderFactory.h"
#include "KLTexturePlayer.h"

float g_offset[] = {1.5f / 1280.0f, 1.5f / 720.0f};

KLFilter4K::KLFilter4K()
{
    _vbo = 0;
    g_offset[0] = 2.5f / KLTexturePlayer::W();
    g_offset[1] = 2.5f / KLTexturePlayer::H();
}

KLFilter4K::~KLFilter4K()
{

}

void KLFilter4K::Init(int w, int h)
{
    GLUtil::CreateFramebuffer(_fb_dat0, w, h);
    GLUtil::CreateFramebuffer(_fb_dat1, w, h);
    GLUtil::CreateFramebuffer(_fb_dat2, w, h);
    GLUtil::CreateFramebuffer(_fb_dat3, w, h);
    GLUtil::CreatePanelVBO(_vbo);
}

void KLFilter4K::ComputeLum(GLuint srcTex)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fb_dat0._fboID);
    glViewport(0, 0, _fb_dat0._w, _fb_dat0._h);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, srcTex);

    auto shader = KLShaderFactory::GetCoreShader(KLE_SHADER_4K_LUM_FILTER);
    GLuint prog = shader->program();
    glUseProgram(prog);
    glUniform1i(glGetUniformLocation(prog, "u_texture0"), 0);

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

void KLFilter4K::PushLighest(GLuint srcTex)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fb_dat1._fboID);
    glViewport(0, 0, _fb_dat1._w, _fb_dat1._h);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, srcTex);

    auto shader = KLShaderFactory::GetCoreShader(KLE_SHADER_4K_LIGHTEST_FILTER);
    GLuint prog = shader->program();
    glUseProgram(prog);
    glUniform1i(glGetUniformLocation(prog, "u_texture0"), 0);
    glUniform2fv( glGetUniformLocation(prog, "offset"), 1, g_offset);

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

void KLFilter4K::ComputeGradient(GLuint srcTex)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fb_dat2._fboID);
    glViewport(0, 0, _fb_dat2._w, _fb_dat2._h);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, srcTex);

    auto shader = KLShaderFactory::GetCoreShader(KLE_SHADER_4K_GRADIENT_FILTER);
    GLuint prog = shader->program();
    glUseProgram(prog);
    glUniform1i(glGetUniformLocation(prog, "u_texture0"), 0);
    glUniform2fv( glGetUniformLocation(prog, "offset"), 1, g_offset);

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

void KLFilter4K::PushGrad(GLuint srcTex)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fb_dat3._fboID);
    glViewport(0, 0, _fb_dat3._w, _fb_dat3._h);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, srcTex);

    auto shader = KLShaderFactory::GetCoreShader(KLE_SHADER_4K_PUSHGRAD_FILTER);
    GLuint prog = shader->program();
    glUseProgram(prog);
    glUniform1i(glGetUniformLocation(prog, "u_texture0"), 0);

    glUniform2fv( glGetUniformLocation(prog, "offset"), 1, g_offset);

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

    glBindFramebuffer(GL_READ_FRAMEBUFFER, _fb_dat3._fboID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    float split = KLTexturePlayer::SplitLine();
    int end = split * _fb_dat3._w -4;
    //int end = _fb_dat3._w;

    glBlitFramebuffer(0, 0, _fb_dat3._w, _fb_dat3._h,
                      0, 0, _fb_dat3._w, _fb_dat3._h,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, KLTexturePlayer::W(), KLTexturePlayer::H());

}

void KLFilter4K::Process(KLTexture* srcTex)
{
    if (_vbo == 0) {
        auto w = KLTexturePlayer::W();
        auto h = KLTexturePlayer::H();
        Init(w, h);
    }

    ComputeLum(srcTex->getTexID());

    PushLighest(_fb_dat0._colorID);

    ComputeGradient(_fb_dat1._colorID);

    PushGrad(_fb_dat2._colorID);
}