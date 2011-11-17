#ifndef __BITMAP_TEXT_HPP__
#define __BITMAP_TEXT_HPP__

namespace HG
{

class Image;

class BitmapText
{
public:
	BitmapText(const char *tmap, bool all_upper);

	~BitmapText();

	void set_image(Image *nimg, unsigned short w, unsigned short h);

	void xoffset(short xoff);
	short xoffset();

	void yoffset(short yoff);
	short yoffset();

	void draw_text(const char *text, float x, float y);
private:
	Image *font_img;
	const char *text_map;

	short offsetX, offsetY;

	unsigned short img_w;
	unsigned short img_h;

	unsigned short glyph_w;
	unsigned short glyph_h;

	unsigned short glyph_rows;
	unsigned short glyph_cols;

	bool upper;
};

}

#endif
