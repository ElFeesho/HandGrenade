/*
 * GLGraphicsEngine.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#include "GLGraphicsEngine.hpp"
#include "../Log.hpp"
#include "GLImage.hpp"

#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <cstdio>

#define glError() { \
    GLenum err = glGetError(); \
    while (err != GL_NO_ERROR) { \
    printf("glError: %s caught at %s:%u", \
           (char*)gluErrorString(err), __FILE__, __LINE__); \
    err = glGetError(); \
    } \
    }

static int attributeList[] = { GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, GLX_RGBA, None };

static Bool WaitForNotify(Display *d, XEvent *e, char *arg)
{
	return (e->type == MapNotify) && (e->xmap.window == (Window) arg);
}

namespace HG
{

GLGraphicsEngine::GLGraphicsEngine()
{
	Log::d("GLGraphicsEngine ctor");
}

GLGraphicsEngine::~GLGraphicsEngine()
{
	Log::d("GLGraphicsEngine dtor");
}

bool GLGraphicsEngine::init(unsigned int w, unsigned int h)
{
	Log::i("GLGraphicsEngine initialising with...");
	Log::i("Screen width:  ", w);
	Log::i("Screen height: ", h);

	dpy = XOpenDisplay(NULL);
	if (!dpy)
	{
		Log::e("Couldn't open the display");
		return false;
	}

	Log::d("Display ptr: ", (void*) dpy);

	/* get an appropriate visual */
	vi = glXChooseVisual(dpy, DefaultScreen(dpy), attributeList);
	if (!vi)
	{
		Log::e("Failed creating visual");
		return false;
	}

	/* create a GLX context */
	cx = glXCreateContext(dpy, vi, 0, GL_FALSE);
	if (!cx)
	{
		return false;
	}
	/* create an X colormap since probably not using default visual */
	cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual,
			AllocNone);
	swa.colormap = cmap;
	swa.border_pixel = 0;
	swa.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;

	glwin = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, w, h, 0,
			vi->depth, InputOutput, vi->visual,
			CWBorderPixel | CWColormap | CWEventMask, &swa);

	Log::d("GLWindow ptr: ", (void*) glwin);
	//XSetStandardProperties(dpy, glwin, "xogl", "xogl", None, NULL, 0, NULL);

	glXMakeCurrent(dpy, glwin, cx);

	XMapWindow(dpy, glwin);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 1);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return true;
}

bool GLGraphicsEngine::shutdown()
{
	Log::i("GLGraphicsEngine shutting down");
	XCloseDisplay(dpy);
	return true;
}

void GLGraphicsEngine::flush()
{
	glXSwapBuffers(dpy, glwin);
	glClear(GL_COLOR_BUFFER_BIT);
	glError();
}

Image *GLGraphicsEngine::open_image(const std::string &filename)
{
	map<string, GLImage*>::iterator image = GLImage::_cached.find(filename);
	if(image == GLImage::_cached.end())
	{
		Log::d("Creating new image for: "+filename);
		GLImage *new_image = new GLImage(filename);
		return new_image;
	}
	(*image).second->ref_count++;


	return (*image).second;
}

void GLGraphicsEngine::destroy_image(Image *img)
{
	GLImage *image = reinterpret_cast<GLImage*>(img);
	if((--image->ref_count) == 0)
	{
		Log::i("Freeing image as refcount has hit 0");
		delete image;
	}
}

void GLGraphicsEngine::draw_image(Image *img, int x, int y, int w, int h, float rot)
{
	if (w == 0)
		w = img->w();
	if (h == 0)
		h = img->h();

	glLoadIdentity();
	glTranslatef(x, y, 0);
	glRotatef(rot, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, img->get_id());
	glEnable(GL_TEXTURE_2D);
	int tint = img->tint();
	int r = (tint & 0xff0000) >> 16;
	int g = (tint & 0xff00) >> 8;
	int b = (tint & 0xff);
	float rf = (float)r/255.0;
	float gf = (float)g/255.0;
	float bf = (float)b/255.0;
	glColor4f(rf, gf, bf, img->alpha());
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 1);
	glTexCoord2f(1, 0);
	glVertex3f(w, 0, 1);
	glTexCoord2f(1, 1);
	glVertex3f(w, h, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0, h, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void GLGraphicsEngine::draw_image_subsection(Image *img, int x, int y, int w, int h, int sx, int sy, int sw, int sh, float rot)
{
	float txl = (float)sx/img->w();
	float txr = (float)(sx+sw)/img->w();
	float tyt = (float)sy/img->h();
	float tyb = (float)(sy+sh)/img->h();

	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, img->get_id());
	glEnable(GL_TEXTURE_2D);
	int tint = img->tint();
	int r = (tint & 0xff0000) >> 16;
	int g = (tint & 0xff00) >> 8;
	int b = (tint & 0xff);
	float rf = (float)r/255.0f;
	float gf = (float)g/255.0f;
	float bf = (float)b/255.0f;
	glColor4f(rf, gf, bf, img->alpha());
	glBegin(GL_QUADS);
	glTexCoord2f(txl, tyt);
	glVertex3f(x, y, 1);
	glTexCoord2f(txr, tyt);
	glVertex3f(x + w, y, 1);
	glTexCoord2f(txr, tyb);
	glVertex3f(x + w, y + h, 1);
	glTexCoord2f(txl, tyb);
	glVertex3f(x, y + h, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void GLGraphicsEngine::draw_sprite(Sprite *sprite, int x, int y)
{
	unsigned int im_w = sprite->get_image()->w();
	unsigned int im_h = sprite->get_image()->h();
	Rect s = sprite->get_section();
	float tex_lx = (float) s.x / im_w;
	float tex_ty = (float) s.y / im_h;

	float tex_rx = (float) (s.x + s.w) / im_w;
	float tex_by = (float) (s.y + s.h) / im_h;
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, sprite->get_image()->get_id());
	glEnable(GL_TEXTURE_2D);
	glColor4f(1, 1, 1, sprite->alpha());
	glBegin(GL_QUADS);
	glTexCoord2f(tex_lx, tex_ty);
	glVertex3f(x, y, 1);
	glTexCoord2f(tex_rx, tex_ty);
	glVertex3f(x + s.w, y, 1);
	glTexCoord2f(tex_rx, tex_by);
	glVertex3f(x + s.w, y + s.h, 1);
	glTexCoord2f(tex_lx, tex_by);
	glVertex3f(x, y + s.h, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void GLGraphicsEngine::draw_line(int x, int y, int x2, int y2, int colour)
{
	glLoadIdentity();
	float a = (float)((colour & 0xff000000) >> 24)/255.0f;
	float r = (float)((colour & 0xff0000) >> 16)/255.0f;
	float g = (float)((colour & 0xff00) >> 8)/255.0f;
	float b = (float)(colour & 0xff)/255.0f;
	
	glColor4f(r, g, b, a);
	glBegin(GL_LINES);
	glVertex3f(x, y, 1);
	glVertex3f(x2, y2, 1);
	glEnd();
	
	
}

void *GLGraphicsEngine::get_screen()
{
	return (void*) dpy;
}

void *GLGraphicsEngine::get_context()
{
	return (void*) glwin;
}

}
