#include "BitmapText.hpp"

#include <cstring>

#include "Engine.hpp"
#include "Image.hpp"
#include "GraphicsEngine.hpp"

namespace HG
{

BitmapText::BitmapText(const char *tmap, bool all_upper) : 
	font_img(NULL),
	text_map(tmap),
	offsetX(0),
	offsetY(0),
	img_w(0),
	img_h(0),
	glyph_w(0),
	glyph_h(0),
	upper(all_upper)
{
}

BitmapText::~BitmapText()
{

}

void BitmapText::set_image(Image *nimg, unsigned short w, unsigned short h)
{
	font_img = nimg;
	glyph_w = w;
	glyph_h = h;

	img_w = font_img->w();
	img_h = font_img->h();

	glyph_rows = img_h / glyph_h;
	glyph_cols = img_w / glyph_w;
}


void BitmapText::xoffset(short xoff)
{
	offsetX = xoff;
}

short BitmapText::xoffset()
{
	return offsetX;
}

void BitmapText::yoffset(short yoff)
{
	offsetY = yoff;
}

short BitmapText::yoffset()
{
	return offsetY;
}

void BitmapText::draw_text(const char *text, float x, float y)
{
	if(!font_img)
		return;

	// Points out the position in the font_img that the
	// next character will come from
	unsigned int _frame;
	float _frameX, _frameY;

	// Stores where the next character will be placed
	float xpos = x;
	float ypos = y;

	for(unsigned int i = 0; i<strlen(text);i++)
	{
		// Newline characters, just jump to the next line and reset the xpos
		if(text[i] == '\n')
		{
			xpos = x;
			ypos += glyph_h+offsetY;
			continue;
		}

		const char *offset;
		if(upper)
		{
			offset = index(text_map,toupper(text[i]));

			// By default, if we find a character not matched, we use it as a space
			if(!offset)
			{
				xpos+=glyph_w+offsetX;
				continue;
			}
			_frame = (unsigned int)(index(text_map,toupper(text[i]))-text_map);
		}
		else
		{
			offset = index(text_map,text[i]);;
			if(!offset)
			{
				xpos+=glyph_w+offsetX;
				continue;
			}
			_frame = (unsigned int)(index(text_map, text[i])-text_map);
		}

		// _frame is now the number of glyphs into the font_img we are
		// using this, it's possible to find the x and y position of the glyph

		font_img->draw_subsection(xpos, ypos, glyph_w, glyph_h, (_frame%glyph_cols)*glyph_w, (_frame/glyph_cols)*glyph_h, glyph_w, glyph_h, 0);

		xpos+=glyph_w+offsetX;
	}
}

}
