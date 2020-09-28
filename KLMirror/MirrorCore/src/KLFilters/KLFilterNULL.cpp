#include "../../include/KLFilters/KLFilterNULL.h"
#include "../../include/KLShaderFactory.h"
#include "../../include/KLTexturePlayer.h"
#include "../../include/AndroidLog.h"

KLFilterNULL::KLFilterNULL()
{
    _vbo = 0;
}

KLFilterNULL::~KLFilterNULL()
{

}

void KLFilterNULL::Init(int w, int h)
{
    GLUtil::CreateFramebuffer(_fb_dat, w, h);
    GLUtil::CreatePanelVBO(_vbo);
}

void KLFilterNULL::Process(GLuint srcTex)
{
    if (_vbo == 0) {
        auto w = KLTexturePlayer::W();
        auto h = KLTexturePlayer::H();
        Init(w, h);
        KLLOG_I("Init Null filter size:%d %d", _fb_dat._w, _fb_dat._h);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, _fb_dat._fboID);
    glViewport(0, 0, _fb_dat._w, _fb_dat._h);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, srcTex);

    auto shader = KLShaderFactory::GetCoreShader(KLE_SHADER_TEXTURE);
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

    glBindFramebuffer(GL_READ_FRAMEBUFFER, _fb_dat._fboID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    float split = KLTexturePlayer::SplitLine();
    //int start = split * _fb_dat._w +2;
    int start = 0;

    glBlitFramebuffer(start, 0, _fb_dat._w, _fb_dat._h,
                      start, 0, _fb_dat._w, _fb_dat._h,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, KLTexturePlayer::W(), KLTexturePlayer::H());
}