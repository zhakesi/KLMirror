#pragma once
#include "KL3D.h"
#include "KLTexture.h"


struct GLFBO_DAT {
    GLuint _fboID;
    GLuint _colorID;
    GLuint _depth;
    int _w;
    int _h;
};

class GLUtil {
private:
    GLUtil() {};
public:
    static void CreatePanelVBO(GLuint &vbo);
    static bool CreateTexture(GLuint &texID);
    static bool CreateTexture(GLuint &texID, int w, int h, KLE_TEX_TYPE type, void *data);
    static bool CreateDepthbuffer(GLuint &depth, int w, int h);
    static bool CreateFramebuffer(GLuint &fbo, GLuint colorTex, GLuint depthTex);
    static void CreateFramebuffer(GLFBO_DAT &dat, int w, int h);
};