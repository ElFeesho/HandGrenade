/*
 * GLImage.h
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#ifndef GLIMAGE_H_
#define GLIMAGE_H_

#include <string>
#include <map>

#include <GL/gl.h>

using std::string;
using std::map;

#include "../Image.hpp"

namespace HG
{

class GLImage: public Image
{
public:
	GLImage();
	GLImage(const string &);


	unsigned int w();
	void width(unsigned int w);

	unsigned int h();
	void h(unsigned int h);

	unsigned int get_id();

	void alpha(float a);
	float alpha();

	void tint(int tint);
	int tint();

private:
	virtual ~GLImage();
	unsigned int _w;
	unsigned int _h;

	int _tint;

	float _a;

	GLuint _texID;

	unsigned int ref_count;

	static map<string, GLImage*> _cached;

	friend class GLGraphicsEngine;
};
}

#endif /* GLIMAGE_H_ */
