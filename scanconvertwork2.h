#include <GLES3/gl3.h>
#include "work.h"

class ScanConvertWork2 : public Work
{
public:
    ScanConvertWork2(GLuint inputTex, GLuint outputTex, const char * vert, const char * frag);
    void Draw(GLuint framebuffer,GLuint vertexbuffer,GLuint uvbuffer);
    GLuint  m_indices2[6];
    GLfloat m_vVertices2[12];
    GLfloat m_vLocs2[16];
};
