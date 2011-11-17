/*
 * GraphicsEngine.h
 *
 * GraphicsEngine functions as an interface for GraphicsEngines that implement it's functionality
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#ifndef GRAPHICSENGINE_H_
#define GRAPHICSENGINE_H_

#include <string>

#include "Image.hpp"
#include "Sprite.hpp"

using std::string;

namespace HG
{

class GraphicsEngine
{
public:
	GraphicsEngine();
	virtual ~GraphicsEngine();

	// Create a graphical context
	virtual bool init(unsigned int w = 800, unsigned int h = 480) = 0;
	virtual bool shutdown() = 0;

	// Flip the screen buffer
	virtual void flush() = 0;

	// Open and return a new image
	virtual Image *open_image(const std::string &filename) = 0;
	virtual void destroy_image(Image *) = 0;

	virtual void draw_image(Image *, int, int, int = 0, int = 0, float = 0.0f) = 0;
	virtual void draw_image_subsection(Image *, int, int, int, int, int, int, int = 0, int = 0, float = 0.0f) = 0;
	virtual void draw_sprite(Sprite *, int, int) = 0;

	virtual void draw_line(int x, int y, int x2, int y2, int colour);

	// This will return the XWindow or the SDL_Surface if applicable
	// this is useful for allow input manager's to be separate from
	// the graphics.
	virtual void *get_screen() = 0;
};

}

#endif /* GRAPHICSENGINE_H_ */
