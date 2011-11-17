/*
 * Image.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#include "Engine.hpp"
#include "GraphicsEngine.hpp"
#include "Image.hpp"

namespace HG
{

Image::Image() {}

Image::~Image() {}

// Convenience drawing methods. 
void Image::draw(int x, int y, int w, int h, float rot)
{
	HG::Engine::get_instance()->get_gfx_engine()->draw_image(this, x, y, w, h, rot);
}

// Specifying no source width and height, uses the same width and height passed in for target
void Image::draw_subsection(int x, int y, int w, int h, int sx, int sy, int sw, int sh, float rot)
{
	HG::Engine::get_instance()->get_gfx_engine()->draw_image_subsection(this, x, y, w, h, sx, sy, sw, sh, rot);
}

Image *Image::open_image(const std::string &imgFile)
{
	return HG::Engine::get_instance()->get_gfx_engine()->open_image(imgFile);
}

void Image::destroy()
{
	HG::Engine::get_instance()->get_gfx_engine()->destroy_image(this);
}

}
