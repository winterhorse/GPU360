extern "C"{

#include <EGL/egl.h>
}
#include <stdio.h>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include "gpu360.h"
#include <GLES3/gl3.h>


        
void GPU360::shutdownEGL(void) {
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);

    display = EGL_NO_DISPLAY;
    surface = EGL_NO_SURFACE;
    context = EGL_NO_CONTEXT;
    printf("shutdown\n");
}

static void checkErr(void)
{
    EGLint eglError = eglGetError(); 
    if (eglError != EGL_SUCCESS) 
    {
        printf("something wrong\n");
    }
}

void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

void GPU360::setupEGL(void) {

    EGLint const attribute_list[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,    // very important!
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,   
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_NONE
    };

        const EGLint surfaceAttr[] = {
            EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
             //EGL_WIDTH, w,
             //EGL_HEIGHT, h,
             EGL_NONE
        };
        NativeWindowType native_window;
        EGLint num_config=0;
        
        EGLNativeDisplayType hDisplayType = fbGetDisplayByIndex(0);
        
        signal(SIGSEGV, handler);   // install our handler
        if(!hDisplayType){
            printf("get display failed\n");
        }
        /* create a native window */
        printf("mark0\n");
        
        /* get an EGL display connection */
        printf("mark1\n");
        display = eglGetDisplay(hDisplayType);
        checkErr();
        printf("mark2\n");
        if(display==EGL_NO_DISPLAY){
            printf("no display\n");
        }
        /* initialize the EGL display connection */
        if(!eglInitialize(display, NULL, NULL)){
            printf("initialization failed\n");
        }
        checkErr();
        eglBindAPI(EGL_OPENGL_ES_API);
        /* get an appropriate EGL frame buffer configuration */
        if(!eglChooseConfig(display, attribute_list, &config, 1, &num_config)){
            printf("choose config failed failed\n");
        }
        checkErr();
        printf("num_config=%d\n", num_config);
         // Get the native visual id that matches the egl config
        EGLint nativeVisualId;
        eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &nativeVisualId);
        checkErr();
        int windowWidth, windowHeight, windowX, windowY;
        fbGetDisplayGeometry(hDisplayType, &windowWidth, &windowHeight);
        windowX = 0;
        windowY = 0;
        //windowWidth = 128;
        //windowHeight = 128;
        
        native_window = fbCreateWindow(hDisplayType, windowX, windowY, windowWidth, windowHeight);
        //printf("windowWidth = %i,\n windowHeight = %i,\n",windowWidth,windowHeight);
        if(!native_window){
            printf("fbCreateWindow failed\n");
        }
        printf("mark3, native_window=0x%x\n", (unsigned int)native_window);
        /* create an EGL window surface */
        surface = eglCreateWindowSurface(display, config, native_window, surfaceAttr);
        checkErr();
        printf("surface=0x%x, EGL_NO_SURFACE=0x%p\n",(unsigned int)native_window,EGL_NO_SURFACE);
          /* create an EGL rendering context */
        EGLint ctxattr[] = { 
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
        };
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxattr);
        printf("context=0x%x\n", (unsigned int)context);
        /* connect the context to the surface */
        if(!eglMakeCurrent(display, surface, surface, context)){
            printf("make current failed\n");
        }
        checkErr();
       
        }

void GPU360::setupTextureAndFBO(void) {
    printf("Setting up Textures and FBOs\n");
    // Create one OpenGL texture

glGenTextures(1, &readTextureID);

// "Bind" the newly created texture : all future texture functions will modify this texture
glBindTexture(GL_TEXTURE_2D, readTextureID);

// Give the image to OpenGL
glTexImage2D(GL_TEXTURE_2D, 0,GL_LUMINANCE, 128, 512, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  


glGenTextures(2, &processTextureID[0]);
glActiveTexture(GL_TEXTURE0+1);
// "Bind" the newly created texture : all future texture functions will modify this texture
glBindTexture(GL_TEXTURE_2D, processTextureID[0]);
// Give the image to OpenGL
glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, 128, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
glActiveTexture(GL_TEXTURE0+2);
// "Bind" the newly created texture : all future texture functions will modify this texture
glBindTexture(GL_TEXTURE_2D, processTextureID[1]);
// Give the image to OpenGL
glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, 128, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


glGenFramebuffers(2, &FramebufferName[0]);
glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName[0]);    
// Set "renderedTexture" as our colour attachement #0
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, processTextureID[0], 0);
glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName[1]); 
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, processTextureID[1], 0);
 //GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};

 //glDrawBuffers(1, DrawBuffers);
//glClear(GL_COLOR_BUFFER_BIT);
printf("ay. GPU ERROR: %i\n", glGetError());
GLuint FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
if(FBOstatus != GL_FRAMEBUFFER_COMPLETE)
            printf("FrameBuffer not ok %i\n",FBOstatus);

}

void GPU360::setupVertexBuffers(void) {
   printf("Setting up Vertex and UV Buffers\n");
    
   g_vertex_buffer_data[0] = -1.0f;
   g_vertex_buffer_data[1] = -1.0f;
   g_vertex_buffer_data[2] = 0.0f;
   
   g_vertex_buffer_data[3] = 1.0f;
   g_vertex_buffer_data[4] = -1.0f;
   g_vertex_buffer_data[5] = 0.0f;
   
   g_vertex_buffer_data[6] = -1.0f;
   g_vertex_buffer_data[7] = 1.0f;
   g_vertex_buffer_data[8] = 0.0f;
   
   g_vertex_buffer_data[9] = -1.0f;
   g_vertex_buffer_data[10] = 1.0f;
   g_vertex_buffer_data[11] =  0.0f;
   
   g_vertex_buffer_data[12] = 1.0f;
   g_vertex_buffer_data[13] = -1.0f;
   g_vertex_buffer_data[14] =  0.0f;
   
   g_vertex_buffer_data[15] = 1.0f;
   g_vertex_buffer_data[16] = 1.0f;
   g_vertex_buffer_data[17] =  0.0f;
   
   g_uv_buffer_data[0] =0.0f; g_uv_buffer_data[1] =0.0f;
   g_uv_buffer_data[2] =1.0f; g_uv_buffer_data[3] =0.0f;
   g_uv_buffer_data[4] =0.0f; g_uv_buffer_data[5] =1.0f;
   g_uv_buffer_data[6] =0.0f; g_uv_buffer_data[7] =1.0f;
   g_uv_buffer_data[8] =1.0f; g_uv_buffer_data[9] =0.0f;
   g_uv_buffer_data[10] =1.0f; g_uv_buffer_data[11] =1.0f;
   
 
// Generate 1 buffer, put the resulting identifier in vertexbuffer
glGenBuffers(1, &vertexbuffer);

glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

glGenBuffers(1, &uvbuffer);
glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
}

void GPU360::setupWork(GLuint inputTex,GLuint outputTex, const char * vert,const char * frag)
{   
   
    packWork = new Work((GLuint)inputTex,outputTex,vert,frag);
    packWork->Draw(FramebufferName[0],vertexbuffer,uvbuffer);
}

GPU360::GPU360(void)
{   printf("GPU constructor\n");
    setupEGL();
}

GPU360::~GPU360()
{   
    printf("GPU destructor\n");
    shutdownEGL();
}





