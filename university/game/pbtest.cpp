#define RES_X 800
#define RES_Y 600

#define MIN_X 0.0
#define MIN_Y 0.0
#define MAX_X 1.0
#define MAX_Y 0.75
#define HALF_MAX_X 0.5
#define HALF_MAX_Y 0.375
#define NEG_HALF_MAX_X -0.5
#define NEG_HALF_MAX_Y -0.375

#define DEPTH 24
#define TEXSIZE 256

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "SDL.h"

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glx.h"
#include "tga.cpp"
#include "drawer.cpp"

typedef struct 
{
	GLXDrawable drawable;
	GLXContext context;
} Context;

GLuint loadNullTexture(GLuint w, GLuint h, GLuint type)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR/*EST*/);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*EST*/);
	glTexImage2D(GL_TEXTURE_2D, 
				 0, type, 
				 w, h, 
				 0, type,
				 GL_UNSIGNED_BYTE,
				 NULL
				);
	printf("Loaded Empty!\n");
	return texture;

}
GLuint loadTexture(char *filename)
{	
	Texture img;
	GLuint texture;
	img.imageData = NULL;
	if(LoadTGA(&img, filename))
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR/*EST*/);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*EST*/);
		glTexImage2D(GL_TEXTURE_2D, 
					 0, img.type, 
					 img.width, img.height, 
					 0, img.type,
					 GL_UNSIGNED_BYTE,
					 img.imageData
					);
		printf("Loaded!\n");
	}
	delete img.imageData;
	return texture;
}

int initVideoMode()
{
   	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8 );
   	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
   	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8 );
   	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
   	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    if( SDL_SetVideoMode(RES_X, RES_Y,  /*video->vfmt->BitsPerPixel*/0, SDL_OPENGL | SDL_FULLSCREEN) == 0 )
	{
		printf("Couldn't set video mode: %s\n", SDL_GetError());
	   	return(1);
	}
    else return(0);
}

void setGLMode()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float halfResX, halfResY;
	halfResX=1;//RES_X/2;
	halfResY=1;//RES_Y/2;

   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
	// Reset The Projection Matrix
	gluOrtho2D(0.0, 1.0, 0.0, 0.75);//*//* left */ -halfResX, /* right */ halfResX, /* bottom */ -halfResY, /* top */ halfResY);
	
   	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(.15, .15, .15, 0.0); 
	glClearDepth(0.0);
	glEnable(GL_TEXTURE_2D);
}
bool defineFBContext(Display *dpy, GLXDrawable drawable, GLXContext context)
{
	drawable = glXGetCurrentDrawable();
	if(drawable == None) 
	{ 
		printf("Can't get current drawable!\n");
		return 1; 
	}
	context = glXGetCurrentContext();
	if(context == NULL) 
	{
		printf("Can't get current context!\n");
		return 1;
	}
	printf("FB Context defined sucessfull!\n");
	return 0;
}
bool definePBContext(Display *dpy, GLXDrawable drawable, GLXContext context, GLXContext shared, const int *sbAttr, const int *pbAttr)
{
	GLXFBConfig* fbc; 
	GLXPbuffer  PBuffer;
	int nElements;
	fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), sbAttr, &nElements); 
	PBuffer = glXCreatePbuffer (dpy, fbc[2], pbAttr); 
	context = glXCreateNewContext( dpy, fbc[2], GLX_RGBA_TYPE, shared, GL_TRUE);
	if(context == NULL)
	{
		printf("Can't define pBuffer context!\n");
		return 1;
	}
	
	printf("PBuffer context defined sucessfull!\n");
}
int main(void)
{
	
   	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 )
	{
       	printf("Couldn't initialize SDL: %s\n", SDL_GetError());
       	exit(1);
	}
	if(initVideoMode()== 1) exit(1);
	printf("set sdl mode!\n");
	GLXFBConfig* fbc; 
	Display *dpy; 
	Window win; 
	GLXContext PBufferCtx; 
	GLXContext WinCtx; 
	GLXPbuffer  PBuffer; 
	int nElements; 
	const int sbAttrib[] = { GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT, GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DOUBLEBUFFER, 0, None }; 
	const int pbAttrib[] = { GLX_PBUFFER_WIDTH, RES_X, GLX_PBUFFER_HEIGHT, RES_Y, None }; 

/*	GLXContext PBContext, FBContext;
	GLXDrawable PBDrawable, FBDrawable;
	const int sbAttr[] = { GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT, GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DOUBLEBUFFER, 0, None }; 
	const int pbAttr[] = { GLX_PBUFFER_WIDTH, RES_X, GLX_PBUFFER_HEIGHT, RES_Y, GLX_PRESERVED_CONTENTS, True,None }; 
	Display *dpy =  glXGetCurrentDisplay(); 
	if(defineFBContext(dpy, FBDrawable, FBContext) != 0) return 1;
	if(definePBContext(dpy, PBDrawable, PBContext, FBContext, sbAttr, pbAttr) != 0) return 1;
*/

	//.... 
	 
	GLuint texID = loadTexture("img.tga");
	//GLuint pbTexID = loadTexture("tex.tga");
	GLuint pbTexID = loadNullTexture(/*RES_Y, RES_Y*/256, 256, GL_RGBA);

	dpy = glXGetCurrentDisplay(); 
	printf("got cur disp!\n");

	fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), sbAttrib, &nElements); 
	printf("got fbconfig!\n");
			//cerr << "NELEMENTS" << nElements << endl; 
	GLXDrawable drawable = glXGetCurrentDrawable(); 
	printf("got drawable!\n");
	dpy = glXGetCurrentDisplay(); 
	printf("got cur disp!\n");
	WinCtx = glXGetCurrentContext();
	printf("got cur cont!\n");

	PBuffer    = glXCreatePbuffer (dpy, fbc[2], pbAttrib); 
	printf("created pbuff!\n");
	PBufferCtx = glXCreateNewContext( dpy, fbc[2], GLX_RGBA_TYPE, WinCtx, GL_TRUE);
	printf("created pbuff context!\n");

	drawer *draw = new drawer(dpy, WinCtx, drawable, PBufferCtx, PBuffer);
	printf("made drawer!\n");
	draw->switchToFrameBuffer();
	printf("swiched to fb!\n");
	setGLMode();
	printf("set gl mode!\n");
	glClear(GL_COLOR_BUFFER_BIT);
	draw->switchToBGBuffer();
	printf("swiched to bb!\n");
	setGLMode();
	printf("set gl mode!\n");
	glClear(GL_COLOR_BUFFER_BIT);
	//.... 
/*
	glXMakeContextCurrent(dpy, PBuffer, PBuffer, PBufferCtx);
	setGLMode();
	
	int i;
	int time, frames = 0;
	time = 0;
	time= SDL_GetTicks()- time;
	for(i = 0; i <= 2000; i++)
	{
		glXMakeContextCurrent(dpy, PBuffer, PBuffer, PBufferCtx);
		drawTexturedQuad(texID, 60, 90, 0);
		glBindTexture(GL_TEXTURE_2D, pbTexID);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, int(RES_X/2) + 200, int(RES_Y/2) + 200, RES_X - 300, RES_Y - 300);//512, 480, 512+int(0.7*TEXSIZE), 480+int(0.7*TEXSIZE));
		//SDL_GL_SwapBuffers();
		//SDL_Delay(4000);

		glXMakeCurrent(dpy,drawable, WinCtx);
		//setGLMode();
		glClear(GL_COLOR_BUFFER_BIT);
		drawTexturedQuad(pbTexID, 0, 0, 35);
		SDL_GL_SwapBuffers();
		time= SDL_GetTicks()- time;
		frames++;
		//SDL_Delay(4000);
		//glXMakeContextCurrent(dpy, PBuffer, PBuffer, PBufferCtx);

	}*/
	SDL_ShowCursor(SDL_DISABLE);
	//SDL_FreeCursor(SDL_GetCursor());
	SDL_Delay(1000);	
	int i;//glClear(GL_COLOR_BUFFER_BIT);
	int time, frames = 0;
	time = 0;
	time= SDL_GetTicks()- time;
	for(i = 0; i <= 1480; i++)
	{
		draw->switchToBGBuffer();
		draw->drawTexQuad(0.0006*i, 0.3, 0.5, (float) i, texID);
		glBindTexture(GL_TEXTURE_2D, pbTexID);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 255, 255);//, 0, RES_Y, RES_Y);//512, 480, 512+int(0.7*TEXSIZE), 480+int(0.7*TEXSIZE));
		draw->switchToFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT);
		draw->drawTexQuad(0.5, 0.375, 0.5, 0.3*i, pbTexID);
		SDL_GL_SwapBuffers();
		time= SDL_GetTicks()- time;
		frames++;
		//SDL_Delay(50);
	}
	//SDL_GL_SwapBuffers();
	SDL_Delay(2000);
	delete draw;
	SDL_Quit();
   	float time_sec= (float)time/1000;
   	printf("\n frames= %i time= %3.3f FPS= %3.3f\n", frames, time_sec, (float)frames/time_sec);
	return 0;
}
