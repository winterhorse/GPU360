#include <GLES3/gl3.h>
#include "work.h"
class GPU360
{
public:
	GPU360();

	~GPU360();
	void setupTextureAndFBO();
	void setupVertexBuffers(void);
	void setupWork(GLuint inputTex,GLuint outputTex, const char * vert,const char * frag);
	GLuint readTextureID;
	GLuint processTextureID[2];
	GLuint FramebufferName[2];
	GLfloat g_vertex_buffer_data[18];
	GLfloat g_uv_buffer_data[18];
	GLuint vertexbuffer;
	GLuint uvbuffer;
	Work *packWork;
	Work *blurWork;
	Work *unpackWork;
private:
    void setupEGL(void);
    void shutdownEGL(void);
    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    EGLSurface surface;
  
};


