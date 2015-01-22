typedef struct
{
	int type;
	int	id;
} BufferInfo;

class drawer
{
	private:
		BufferInfo curBuffer;
		int renderDepth; // depth of glBegin...glEnd: 0 - not rendering, 1 rendering, '> 1' or '< 1' is pretty bad
		int listDepth; // depth of render lists
		Display *display; // current X display
		GLXContext frameBufferContext, /* fb OGL-context */ bgBufferContext; /* background (pBuffer) OGL-context */
		GLXDrawable frameBufferDrawable, bgBufferDrawable;

	public:
		////////	
		// Public methods
		////////////////
		void drawTexQuad(GLfloat x, GLfloat y, GLfloat scaleVal, GLfloat rotateVal, GLuint texID); // draw textured quad
		GLuint getCurBuffer(); // get current render buffer
		bool switchToFrameBuffer(); // set framebuffer as a cur. render buffer
		//switchToTexBuffer(); // switch to texture render pBuffer
		bool switchToBGBuffer(); // switch to background render buffer
		//getTexFromTexBuffer(); // get fresh-rendered texture from tex. pBuffer
		void startGLList(); // start OGL render list
		void endGLList(); // end OGL render list
		void drawGLList(); // draw OGL render list
		//startRender(); //glBegin
		//endRender(); //glEnd
		void getTexFromBGBuffer(); // get backgroun textures
		drawer(Display *dpy, GLXContext fbCtx, GLXDrawable fbDrw, GLXContext bgCtx, GLXDrawable bgDrw);
		~drawer();
};
