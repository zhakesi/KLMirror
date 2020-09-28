#include "../../include/KLShader.h"
#include "../../include/AndroidLog.h"
#include "../../include/KLFile.h"

static GLuint LoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader;
    GLint compiled;
    shader = glCreateShader(type);
    if (shader == 0)
        return 0;
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    glGetShaderiv (shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char *infoLog = new char[infoLen];
            glGetShaderInfoLog( shader, infoLen, NULL, infoLog );
            KLLOG_E("Error compiling shader:\n%s\n", infoLog);
            delete[] infoLog;
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

KLShader::KLShader(KLE_SHADER_TYPE type)
{
    _type = type;
}

bool KLShader::createProgram(const char *str_vertex, const char *str_frag)
{
    GLuint vertex_shader = LoadShader(GL_VERTEX_SHADER, str_vertex);
    GLuint frag_shader = LoadShader(GL_FRAGMENT_SHADER, str_frag);

    _program = glCreateProgram();
    if ( _program == 0 ) return false;
    glAttachShader (_program, vertex_shader);
    glAttachShader (_program, frag_shader);
    glLinkProgram (_program);

    GLint linked;
    glGetProgramiv (_program, GL_LINK_STATUS, &linked );
    if (!linked )
    {
        GLint infoLen = 0;
        glGetProgramiv ( _program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char* infoLog = new char[infoLen];
            glGetProgramInfoLog ( _program, infoLen, NULL, infoLog );
            KLLOG_E("Error compiling shader:\n%s\n", infoLog);
            delete[] infoLog;
        }
        glDeleteProgram (_program);
        return false;
    }
    // KLLOG_I("Create GL Program:%x", _program);
    return true;
}

bool KLShader::createProgramFromFile(const char *path)
{
    int length;
    KLFileIStream reader;
    reader.open(path, length);
    char *buf = new char[length +1];
    memset(buf, 0, length +1);
    reader.read(buf, length);
    std::string str_shader(buf);
    delete []buf;

    size_t index0 = str_shader.find("#version");
    size_t index1 = str_shader.find("#version", index0 + 1);

    std::string vertex = str_shader.substr(index0, index1);
    std::string frag = str_shader.substr(index1, length);
//    KLLOG_I("index0 =%d, index1=%d\n", index0, index1);
//    KLLOG_I("vertex string:\n%s\n", vertex.c_str());
//    KLLOG_I("frag string:\n%s\n", frag.c_str());
    return createProgram(vertex.c_str(), frag.c_str());
}

GLuint KLShader::program()
{
    return _program;
}
