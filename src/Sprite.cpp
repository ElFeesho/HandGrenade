/*
 * Sprite.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#include "Sprite.hpp"
#include "Engine.hpp"
#include "GraphicsEngine.hpp"

#include <fstream>

#include "Log.hpp"

using std::ifstream;

namespace HG
{

Sprite::Sprite() :
	_a(1), frames(0), cframe(0), anim_mode(0), _section(0, 0, 0, 0)
{
	ltick = 0;
}

Sprite::Sprite(const string &sprite_file) :
	anim_mode(0), _section(0, 0, 0, 0)
{
	ltick = 0;
	load_sprite(sprite_file);
}

Sprite::~Sprite()
{
	delete sheet;
	Log::d("Sprite dtor");
}

void Sprite::load_sprite(const string &sprite_file)
{
	ifstream sprite_handle(sprite_file.c_str());

	if(!sprite_handle.good())
	{
		Log::e("Couldn't open sprite: " + sprite_file);
		return;
	}

	string sprite_img_file;

	sprite_handle >> sprite_img_file;
	Log::i("Sprite image: "+sprite_img_file);

	sheet = Image::open_image(sprite_img_file);

	int width = 0, height = 0;

	sprite_handle >> width;
	sprite_handle >> height;

	Log::i("Sprite Width: ", width);
	Log::i("Sprite Height: ", height);

	_section.w = width;
	_section.h = height;

	frames = (sheet->w() / width) * (sheet->h() / height);
	cframe = 1;
	Log::i("Sprite frames: ",frames);
	w = sheet->w();
	h = sheet->h();

	xscale = 1.0f;
	yscale = 1.0f;

	sprite_handle >> fticks;
	sprite_handle.close();

	Log::i("Sprite fps: ", 1000.0/(float)fticks);
}

void Sprite::update()
{
	if (Engine::get_instance()->get_ticks() > ltick)
	{
		cframe++;

		// Update the _section that will be used by the render engine
		_section.x = cframe * _section.w - ((cframe * _section.w) / w) * w;

		_section.y = ((cframe * _section.w) / w) * _section.h;
		ltick += fticks;
		if (cframe >= frames)
			cframe = 0;
	}
}

Rect Sprite::get_section()
{
	return _section;
}

Image *Sprite::get_image()
{
	return sheet;
}

void Sprite::alpha(float a)
{
	_a = a;
}

float Sprite::alpha()
{
	return _a;
}

// Convenience method
void Sprite::draw(int x, int y)
{
	HG::Engine::get_instance()->get_gfx_engine()->draw_sprite(this, x, y);
}

}
