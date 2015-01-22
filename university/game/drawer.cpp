#include "drawer.h"

void drawer::drawTexQuad(GLfloat x, GLfloat y, GLfloat scaleVal, GLfloat rotateVal, GLuint texID)
{
	glLoadIdentity(); // Load '1' matrix
	glTranslatef(x, y, 0.0); // Move quad to (x; y)
	glRotatef(rotateVal, 0.0, 0.0, 1.0); // Rotate by rotateVal angle
	glScalef(scaleVal, scaleVal, 1.0); // scale quad

	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glBegin(GL_QUADS);
	
		// top left
		glTexCoord2f(0.0, 0.0);
		glVertex2f(NEG_HALF_MAX_Y, NEG_HALF_MAX_Y);

		// top right
		glTexCoord2f(1.0, 0.0);
		glVertex2f(HALF_MAX_Y, NEG_HALF_MAX_Y);

		// bottom right
		glTexCoord2f(1.0, 1.0);
		glVertex2f(HALF_MAX_Y, HALF_MAX_Y);

		// bottom left
		glTexCoord2f(0.0, 1.0);
		glVertex2f(NEG_HALF_MAX_Y, HALF_MAX_Y);
	
	glEnd();
}
drawer::drawer(Display *dpy, GLXContext fbCtx, GLXDrawable fbDrw, GLXContext bgCtx, GLXDrawable bgDrw)
{
	display = dpy;
	frameBufferContext = fbCtx;
	bgBufferContext = bgCtx;
	frameBufferDrawable = fbDrw;
	bgBufferDrawable = bgDrw;
}
bool drawer::switchToBGBuffer()
{
	//glXMakeContextCurrent(display, bgBufferDrawable, bgBufferDrawable, bgBufferContext);
	glXMakeCurrent(display, bgBufferDrawable, bgBufferContext);
}
bool drawer::switchToFrameBuffer()
{
	glXMakeCurrent(display, frameBufferDrawable, frameBufferContext);
}
drawer::~drawer()
{
	glXDestroyContext(display, bgBufferContext);
};
