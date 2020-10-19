#include "../../include/KLFilters/KLColorMapFilter.h"
#include "GLUtil.h"
#include "KLImage.h"
#include "KLShaderFactory.h"
#include "KLTexturePlayer.h"

KLColorMapFilter::KLColorMapFilter()
{
    _vbo = 0;
    _lut_tex = nullptr;
}
void KLColorMapFilter::Init()
{
    KLImage image;
    image.load_from_file("lut.png");
    _lut_tex = new KLTexture;
    _lut_tex->updateTexIMG(&image);
    image.clear();

    GLUtil::CreatePanelVBO(_vbo);
}

void KLColorMapFilter::Process(GLuint srcTex)
{
    if (_vbo == 0) {
        Init();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, KLTexturePlayer::W(), KLTexturePlayer::H());
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