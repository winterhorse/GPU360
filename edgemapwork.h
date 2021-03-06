#include <GLES3/gl3.h>
#include "work.h"

class EdgeMapWork : public Work
{
public:
    EdgeMapWork(GLuint inputTex, GLuint outputTex, const char * vert,const char * frag, GLuint inputTex2);
    void Draw(GLuint framebuffer,GLuint vertexbuffer,GLuint uvbuffer);
    GLuint inputTexture2;
};
