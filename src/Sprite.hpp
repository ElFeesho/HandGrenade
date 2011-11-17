/*
 * Sprite.h
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#ifndef SPRITE_H_
#define SPRITE_H_

#include <string>

using std::string;

#include "Image.hpp"
#include "Rect.hpp"

namespace HG
{

class Sprite
{
public:
	Sprite();
	Sprite(const string&);
	virtual ~Sprite();

	void update();

	Rect get_section();
	Image *get_image();

	void alpha(float a);
	float alpha();

	void draw(int x, int y);
private:
	void load_sprite(const string &sprite_file);
	Image *sheet;

	int w;
	int h;

	float _a;

	float xscale;
	float yscale;

	Rect _section;

	int frames;
	int cframe;

	long ltick;
	long fticks;

	int anim_mode; // 0 normal, 1 bounce
};
}

#endif /* SPRITE_H_ */
