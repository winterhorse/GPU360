#include <GLES3/gl3.h>
#define PACK_MAX			255.0F
#define	PACK_MIN			-256.0F
class Work 
{
public:
    //ConvWorker();
    Work(GLuint inputTex, GLuint outputTex, const char * vert,const char * frag);
    ~Work();
    void Draw(GLuint framebuffer,GLuint vertexbuffer,GLuint uvbuffer);
    GLuint inputTexture;
    GLuint outputTexture;
    GLuint Program;
};
