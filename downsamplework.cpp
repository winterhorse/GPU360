extern "C"{

#include <EGL/egl.h>
}
#include <stdio.h>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include "downsamplework.h"
#include <GLES3/gl3.h>
#include "loadShader.h"
#include <math.h>

DownSampleWork::DownSampleWork(GLuint inputTex, GLuint outputTex, const char * vert,const char * frag) : Work(inputTex, outputTex, vert, frag) {}

void DownSampleWork::Draw(GLuint framebuffer,GLuint vertexbuffer,GLuint uvbuffer)
{   
    glViewport(0,0,TEXTURE_WIDTH/2,TEXTURE_HEIGHT/2);
    glUseProgram(Program);
    GLuint BlurTextureLocation = glGetUniformLocation(Program, "myTextureSampler");
// Draw triangle...

    
    glUniform1f(glGetUniformLocation( Program, "maxF"), PACK_MAX);
    glUniform1f(glGetUniformLocation( Program, "minF"), PACK_MIN);
     
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE, 
        0,                  // stride
        (void*)0            // array buffer offset
    );
 
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layou
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
    );

//glBindFramebuffer(GL_FRAMEBUFFER, 0);
//glBindTexture(GL_TEXTURE_2D,0);
glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);   

glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, outputTexture, 0);
glClear(GL_COLOR_BUFFER_BIT);
//printf("A. GPU ERROR: %i\n", glGetError() );
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, inputTexture);

glUniform1i(BlurTextureLocation, 0);

glDrawArrays(GL_TRIANGLES, 0, 6); 
//dump_image_DS("DownSample.dat");
//printf("DownSample\n");
//glBindFramebuffer(GL_FRAMEBUFFER, 0);
//glBindTexture(GL_TEXTURE_2D,0);
}
