/*
 * GLGraphicsScene.h
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#ifndef GLGRAPHICSSCENE_H_
#define GLGRAPHICSSCENE_H_

#include <X11/Xlib.h>
#include <GL/glx.h>

#include "../GraphicsEngine.hpp"

namespace HG
{

class GLGraphicsEngine: public GraphicsEngine
{
public:
	GLGraphicsEngine();
	virtual ~GLGraphicsEngine();

	bool init(unsigned int w = 800, unsigned int h = 480);
	bool shutdown();

	void flush();

	Image *open_image(const std::string &filename);
	void destroy_image(Image *img);


	void draw_image(Image *img, int x, int y, int w, int h, float r = 0);
	void draw_image_subsection(Image *img, int x, int y, int w, int h, int sx, int sy, int sw, int sh, float r = 0);
	void draw_sprite(Sprite *sprite, int x, int y);

	void draw_line(int x, int y, int x2, int y2, int colour);

	void *get_screen();
	void *get_context();
private:
	Display *dpy;
	Window glwin;
	XVisualInfo *vi;
	Colormap cmap;
	XSetWindowAttributes swa;
	GLXContext cx;
};

}

#endif /* GLGRAPHICSSCENE_H_ */
