#include "../../include/GLUtil.h"
#include "../../include/AndroidLog.h"

void GLUtil::CreatePanelVBO(GLuint &vbo)
{
    glGenBuffers(1, &vbo);
    GLfloat vertices[] = {
            0.0f, 1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 0.0f, 1.0f,

            1.0f, 1.0f, 1.0f, 0.0f,

            1.0f, 0.0f, 1.0f, 1.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool GLUtil::CreateTexture(GLuint &texID)
{
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    return true;
}

bool GLUtil::CreateTexture(GLuint &texID, int w, int h,
        KLE_TEX_TYPE type, void *data)
{
    bool ret = true;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    switch (type) {
        case KLE_TEX_RGBA:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        case KLE_TEX_LUM:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w, h,
                         0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
            break;
        case KLE_TEX_UV:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8_EXT, w/2, h/2,
                         0, GL_RG8_EXT, GL_UNSIGNED_BYTE, data);
            break;
        default:
            assert(0);
            break;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return ret;
}
bool GLUtil::CreateDepthbuffer(GLuint &depth, int w, int h)
{
    glGenRenderbuffers(1, &depth);
    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    return true;
}

bool GLUtil::CreateFramebuffer(GLuint &fbo, GLuint colorTex, GLuint depth)
{
    GLint pre_fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &pre_fbo);
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, colorTex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        KLLOG_E("Create Framebuffer Failed!\n");
        assert(0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, pre_fbo);
    // KLLOG_I("Create Framebuffer OK\n");
    return true;
}

void GLUtil::CreateFramebuffer(GLFBO_DAT &dat, int w, int h)
{
    dat._w = w;
    dat._h = h;

    glGenRenderbuffers(1, &dat._depth);
    glBindRenderbuffer(GL_RENDERBUFFER, dat._depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenTextures(1, &dat._colorID);
    glBindTexture(GL_TEXTURE_2D, dat._colorID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenFramebuffers(1, &dat._fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, dat._fboID);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, dat._colorID, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, dat._depth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        KLLOG_E("Create Framebuffer Failed!\n");
        assert(0);
    }
}