extern "C"{

#include <EGL/egl.h>
}
#include <stdio.h>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include "work.h"
#include <GLES3/gl3.h>
#include "loadShader.h"

int Work::dump_image(const char *name)//, float *dest, unsigned int nbytes)
{
    FILE * fp;
    fp = fopen(name,"wb");
    GLuint p[TEXTURE_WIDTH*TEXTURE_HEIGHT];
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels( 0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, p);
    printf("Read Pixels ERROR: %i\n", glGetError() );
    fwrite(&p, 4*TEXTURE_WIDTH*TEXTURE_HEIGHT, 1, fp);
    fclose(fp); 
    return 0;
}

int Work::dump_image_DS(const char *name)//, float *dest, unsigned int nbytes)
{
    FILE * fp;
    fp = fopen(name,"wb");
    GLuint p[(TEXTURE_WIDTH/2)*(TEXTURE_HEIGHT/2)];
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels( 0, 0, TEXTURE_WIDTH/2, TEXTURE_HEIGHT/2, GL_RGBA, GL_UNSIGNED_BYTE, p);
    printf("Read Pixels ERROR: %i\n", glGetError() );
    fwrite(&p, 4*(TEXTURE_WIDTH/2)*(TEXTURE_HEIGHT/2), 1, fp);
    fclose(fp); 
    return 0;
}

void Work::Draw(GLuint framebuffer,GLuint vertexbuffer,GLuint uvbuffer){}


Work::Work(GLuint inputTex, GLuint outputTex, const char * vert,const char * frag)
{   
    printf("Work constructor\n");
    inputTexture = inputTex;
    outputTexture = outputTex;
    Program = LoadShaders(vert,frag);
    // GetUniforms
   
}
   

Work::~Work()
{   
    printf("Work destructor\n");
}

