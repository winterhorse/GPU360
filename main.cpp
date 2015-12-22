extern "C"{
#include <stdlib.h>
#include <unistd.h>
#include <EGL/egl.h>
//#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <signal.h>
#include <execinfo.h>
#include <stdlib.h>
#include <string.h>
#include <main.h>
#include <errno.h>
#include <fcntl.h>

/* system time include*/
#include <sys/time.h>
}
#include "gpu360.h"
#include <ctime>
#include "loadShader.h"
static EGLint const attribute_list[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,    // very important!
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,   
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_NONE
};

extern char * textFileRead(const char *);

int read_file(const char *name, char *dest, unsigned int nbytes)
{   
    int fd;
    unsigned int r, pos, chunk;
	
    fd=open(name, O_RDONLY);
    if(fd<0){
	return -1;
    }
    for(pos=0;pos<nbytes;){
	chunk=nbytes-pos;
	chunk=(chunk>4096)?4096:chunk;
	r=read(fd, (void *)&dest[pos], chunk);
	if(r<0){
		printf("Read error :%s:\n", strerror(errno));
		close(fd);
		return -1;
	}else if(r==0){
		return 0;
	}else{
	    //printf("%i \n",pos);
		pos=pos+r;
	}
    }
    close(fd);
    return 0;
}

int dump_image(const char *name)//, float *dest, unsigned int nbytes)
{
    FILE * fp;
    fp = fopen(name,"wb");
    GLuint p[128*512];
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels( 0, 0, 128, 512, GL_RGBA, GL_UNSIGNED_BYTE, p);
    printf("Read Pixels ERROR: %i\n", glGetError() );
    fwrite(&p, 4*128*512, 1, fp);
    fclose(fp); 
    return 0;
}

//static unsigned char dummyData[4*w*h];

int main(int argc, char ** argv)
{      
GPU360 *gpu;
gpu=new GPU360();     
//gpu->setupEGL();
gpu->setupTextureAndFBO();
unsigned char scan0[128*512];
//__fp16 scan0f[128*512];
//GLint scan0I[128*512];
read_file("envIn.bin", (char *) scan0, 128*512);
//read_float("envInFloat.bin", (float *) scan0, sizeof(float)*128*512);

//for( int i=0;i<512*128;i++)
//    scan0f[i] = (__fp16) 65000.0*scan0[i];
    
//for( int i=0;i<512*128;i++)
  // scan0I[i] = i;//(GLshort) 65000.0*scan0[i];
    
//GLuint VertexArrayID;
//glGenVertexArrays(1, &VertexArrayID);
//glBindVertexArray(VertexArrayID);

glBindTexture(GL_TEXTURE_2D, gpu->readTextureID);

// Give the image to OpenGL
glTexImage2D(GL_TEXTURE_2D, 0,GL_LUMINANCE, 128, 512, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, scan0);





            
         
 // An array of 6 vectors which represents 6 vertices

                              

/*static const GLfloat g_vertex_buffer_data[] = { 
		 -1.0f,  -1.0f, 0.0f,
		 1.0f,  -1.0f, 0.0f,
		 -1.0f,  1.0f, 0.0f,
		 -1.0f,  1.0f, 0.0f,
		 1.0f,  -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
};

static const GLfloat g_uv_buffer_data[] = {
		0.0f, 0.0f, 
		1.0f, 0.0f, 
		0.0f, 1.0f, 
		0.0f, 1.0f, 
		1.0f, 0.0f, 
		1.0f, 1.0f,  
        };*/                  
// This will identify our vertex buffer
// Create and compile our GLSL program from the shaders     

gpu->setupVertexBuffers();


//GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
//GLuint Lum2RGBAProg = LoadShaders( "lumToRGBA.vertexshader", "lumToRGBA.fragmentshader" );
GLuint RenderFloatProg = LoadShaders( "renderFloatRGBA.vertexshader", "renderFloatRGBA.fragmentshader");
//GLuint program2ID = LoadShaders( "GradVertexShader.vertexshader", "GradFragmentShader.fragmentshader" );
//GLuint blurProg = LoadShaders( "sepConv3x3.vertexshader", "sepConv3x3.fragmentshader" );
GLuint blurProg = LoadShaders( "sepConv5x5.vertexshader", "sepConv5x5.fragmentshader" );
//GLuint blurProg = LoadShaders( "sepConv7x7.vertexshader", "sepConv7x7.fragmentshader" );
//GLuint blurProg = LoadShaders( "sepConv9x9.vertexshader", "sepConv9x9.fragmentshader" );
//GLuint blurProg = LoadShaders( "sepConv11x11.vertexshader", "sepConv11x11.fragmentshader" );
//GLuint blurProg = LoadShaders( "Conv3x3.vertexshader", "Conv3x3.fragmentshader" );
//GLuint blurProg = LoadShaders( "Conv5x5.vertexshader", "Conv5x5.fragmentshader" );
//GLuint BlurTextureLocation = glGetUniformLocation(blurProg, "myTextureSampler");
// Give our vertices to OpenGL.

gpu->setupVertexBuffers();
 /*printf("1. GPU ERROR: %i\n", glGetError() );
  printf("New Main\n");
 
 glViewport(0,0,128,512);
// 1rst attribute buffer : vertices
//    do{
glClearColor(0.6f, 0.5f, 1.0f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// Use our shader


glUseProgram(Lum2RGBAProg);
GLuint LuminanceTextureLocation = glGetUniformLocation(Lum2RGBAProg, "myTextureSampler");

glUniform1f(glGetUniformLocation( Lum2RGBAProg, "maxF"), PACK_MAX);
glUniform1f(glGetUniformLocation( Lum2RGBAProg, "minF"), PACK_MIN);


glBindFramebuffer(GL_FRAMEBUFFER, 0);
glBindFramebuffer(GL_FRAMEBUFFER, gpu->FramebufferName[0]);   
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, gpu->processTextureID[0], 0);


glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, gpu->readTextureID);
glUniform1i(LuminanceTextureLocation, 0);


glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, gpu->vertexbuffer);
glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE, 
    0,                  // stride
    (void*)0            // array buffer offset
 );
glEnableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER, gpu->uvbuffer);
glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layou
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
);
		
// Draw the triangle !
// Set our "myTextureSampler" sampler to user Texture Unit 0

printf("2. GPU ERROR: %i\n", glGetError() );
glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
printf("3. GPU ERROR: %i\n", glGetError() );

glDisableVertexAttribArray(0);
glDisableVertexAttribArray(1);*/

gpu->setupWork(gpu->readTextureID,gpu->processTextureID[0], "lumToRGBA.vertexshader","lumToRGBA.fragmentshader");

dump_image("Dump1.dat");


	time_t t1;
	time_t t2;

	time(&t1);

/////////////////////////// Filter ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////  Blur Axially   //////////////////////////////////
int iterations = 1;
	for (int i=0;i<iterations;i++)
	{
glViewport(0,0,128,512);
glUseProgram(blurProg);
GLuint BlurTextureLocation = glGetUniformLocation(blurProg, "myTextureSampler");
// Draw triangle...
glUniform1f(glGetUniformLocation( blurProg, "texelWidth"), 1.0/128.0); 
glUniform1f(glGetUniformLocation( blurProg, "texelHeight"), 1.0/512.0); 
glUniform1f(glGetUniformLocation( blurProg, "convAxial"), 1.0); 
glUniform1f(glGetUniformLocation( blurProg, "convLateral"), 0.0); 

glUniform1f(glGetUniformLocation( blurProg, "convKernel_0"), 0.2); 
glUniform1f(glGetUniformLocation( blurProg, "convKernel_1"), 0.2); 
glUniform1f(glGetUniformLocation( blurProg, "convKernel_2"), 0.2);
glUniform1f(glGetUniformLocation( blurProg, "convKernel_3"), 0.2);
glUniform1f(glGetUniformLocation( blurProg, "convKernel_4"), 0.2);


glUniform1f(glGetUniformLocation( blurProg, "maxF"), PACK_MAX);
glUniform1f(glGetUniformLocation( blurProg, "minF"), PACK_MIN);

glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, gpu->vertexbuffer);
glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE, 
    0,                  // stride
    (void*)0            // array buffer offset
 );
 
glEnableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER, gpu->uvbuffer);
glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layou
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
);

glBindFramebuffer(GL_FRAMEBUFFER, 0);
glBindTexture(GL_TEXTURE_2D,0);
glBindFramebuffer(GL_FRAMEBUFFER, gpu->FramebufferName[1]);   
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, gpu->processTextureID[1], 0);

glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, gpu->processTextureID[0]);

glUniform1i(BlurTextureLocation, 0);


glDrawArrays(GL_TRIANGLES, 0, 6); 

dump_image("Dump2.dat");


///////////////////////////  Blur Laterally   //////////////////////////////////

glUseProgram(blurProg);
BlurTextureLocation = glGetUniformLocation(blurProg, "myTextureSampler");
// Draw triangle...
glUniform1f(glGetUniformLocation( blurProg, "texelWidth"), 1.0/128.0); 
glUniform1f(glGetUniformLocation( blurProg, "texelHeight"), 1.0/512.0); 
glUniform1f(glGetUniformLocation( blurProg, "convAxial"), 0.0); 
glUniform1f(glGetUniformLocation( blurProg, "convLateral"), 1.0); 

glUniform1f(glGetUniformLocation( blurProg, "convKernel_0"), 0.2); 
glUniform1f(glGetUniformLocation( blurProg, "convKernel_1"), 0.2); 
glUniform1f(glGetUniformLocation( blurProg, "convKernel_2"), 0.2);
glUniform1f(glGetUniformLocation( blurProg, "convKernel_3"), 0.2);
glUniform1f(glGetUniformLocation( blurProg, "convKernel_4"), 0.2);



glUniform1f(glGetUniformLocation( blurProg, "maxF"), PACK_MAX);
glUniform1f(glGetUniformLocation( blurProg, "minF"), PACK_MIN);

glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, gpu->vertexbuffer);
glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE, 
    0,                  // stride
    (void*)0            // array buffer offset
 );
 
glEnableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER, gpu->uvbuffer);
glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layou
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
);

glBindFramebuffer(GL_FRAMEBUFFER, 0);
glBindTexture(GL_TEXTURE_2D,0);
glBindFramebuffer(GL_FRAMEBUFFER, gpu->FramebufferName[1]);   
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, gpu->processTextureID[0], 0);

glActiveTexture(GL_TEXTURE0+2);
glBindTexture(GL_TEXTURE_2D, gpu->processTextureID[1]);

glUniform1i(BlurTextureLocation, 2);


glDrawArrays(GL_TRIANGLES, 0, 6); 

dump_image("Dump3.dat");

}
/////////////////////////// Render to screen  /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

time(&t2);

double seconds = difftime(t2,t1);

printf("time per iteration = %f milliseconds\n", 1000.0f*seconds/(float)iterations);


	
glViewport(0,0,128,512);
//glUseProgram(programID);
//GLuint TextureLocation = glGetUniformLocation(programID, "myTextureSampler");

glUseProgram(RenderFloatProg);
GLuint PackedTexture = glGetUniformLocation(RenderFloatProg, "myTextureSampler");

glUniform1f(glGetUniformLocation( RenderFloatProg, "maxF"), PACK_MAX);
glUniform1f(glGetUniformLocation( RenderFloatProg, "minF"), PACK_MIN);

glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, gpu->vertexbuffer);
glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE, 
    0,                  // stride
    (void*)0            // array buffer offset
 );
 
glEnableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER, gpu->uvbuffer);
glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layou
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
);

glBindFramebuffer(GL_FRAMEBUFFER, 0);
glBindTexture(GL_TEXTURE_2D,0);
glBindFramebuffer(GL_FRAMEBUFFER, gpu->FramebufferName[0]);   
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, gpu->processTextureID[1], 0);

glActiveTexture(GL_TEXTURE0+1);
glBindTexture(GL_TEXTURE_2D, gpu->processTextureID[0]);

glUniform1i(PackedTexture, 1);


glDrawArrays(GL_TRIANGLES, 0, 6); 

dump_image("Dump4.dat");

glDisableVertexAttribArray(0);
glDisableVertexAttribArray(1);


        //eglSwapBuffers(display, surface);
        delete(gpu);
        return EXIT_SUCCESS;
}

