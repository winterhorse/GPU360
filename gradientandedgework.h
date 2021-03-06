#include <GLES3/gl3.h>
#include "work.h"

class GradientAndEdgeWork : public Work
{
public:
    GradientAndEdgeWork(GLuint inputTex, GLuint outputTex, const char * vert,const char * frag, GLuint outputTex2, GLuint outputTex3, float * kernel3x3);
    void Draw(GLuint framebuffer,GLuint vertexbuffer,GLuint uvbuffer);
    GLuint outputTexture2;
    GLuint outputTexture3;
    float * kernel;
};
