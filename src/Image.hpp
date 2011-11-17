/*
 * Image.h
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <string>

namespace HG
{

class Image
{
public:
	Image();
	virtual ~Image();

	// Dimension methods
	virtual unsigned int w() = 0;
	virtual void width(unsigned int w) = 0;

	virtual unsigned int h() = 0;
	virtual void h(unsigned int h) = 0;

	virtual unsigned int get_id() { return 0; }

	virtual void alpha(float) = 0;
	virtual float alpha() = 0;

	virtual void tint(int tint) = 0;
	virtual int tint() = 0;

	void draw(int, int, int = 0, int = 0, float = 0.0f);
	void draw_subsection(int, int, int, int, int, int, int = 0, int = 0, float = 0.0f);

	// Factory methods
	static Image *open_image(const std::string &imgFile);
	void destroy();
};
}

#endif /* IMAGE_H_ */
