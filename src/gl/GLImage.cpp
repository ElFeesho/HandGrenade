/*
 * GLImage.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#include <cstdio>
#include <png.h>
#include <setjmp.h>

#include "../Log.hpp"
#include "GLImage.hpp"
#include <GL/glu.h>

#define glError() { \
    GLenum err = glGetError(); \
    while (err != GL_NO_ERROR) { \
    printf("glError: %s caught at %s:%u", \
           (char*)gluErrorString(err), __FILE__, __LINE__); \
    err = glGetError(); \
    } \
    }

using namespace std;

void png_error_fn(png_struct *png_ptr, const char *details)
{
	HG::Log::e("PNG Error Occurred: ", details);
}

void png_warn_fn(png_struct *png_ptr, const char *details)
{
	HG::Log::w("PNG Warning Occurred: ", details);
}

namespace HG
{

GLImage::GLImage()
{
	HG::Log::d("GLImage ctor");
}

GLImage::GLImage(const string &filename) :
	_tint(0xffffffff), _texID(0), ref_count(1), _a(1)
{
	HG::Log::d("GLImage ctor");
	FILE *fin = fopen(filename.c_str(), "r");
	if (fin)
	{
		png_byte header[8] = { 0 };
		fread(reinterpret_cast<char*> (header), 1, 8, fin);

		if (png_sig_cmp(header, 0, 8) != 0)
		{
			Log::e("Invalid PNG File: ", filename);
			return;
		}
	}
	else
	{
		Log::e("Couldn't open: ", filename);
		return;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
			(png_voidp) NULL, png_error_fn, png_warn_fn);
	if (!png_ptr)
	{
		fclose(fin);
		Log::e("Couldn't allocate PNG read struct");
		return;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fin);
		Log::e("Couldn't allocate PNG info struct");
		return;
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fin);
		Log::e("Couldn't allocate PNG info struct");
		return;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fin);
		Log::e("Couldn't setjmp");
		return;
	}

	png_init_io(png_ptr, fin);
	png_set_sig_bytes(png_ptr, 8);
	png_read_png(png_ptr, info_ptr, 0, NULL);

	_w = png_get_image_width(png_ptr, info_ptr);
	_h = png_get_image_height(png_ptr, info_ptr);
	// BYTES per pixel, not bits...
	int bpp = png_get_channels(png_ptr, info_ptr);

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	Log::i("Read png successfully");

	glGenTextures(1, &_texID);
	glBindTexture(GL_TEXTURE_2D, _texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/*
	 * To get around the power of 2 problem, when copying png data
	 * a buffer is created that has the size of

	unsigned int tex_size = 1;
	while (tex_size < _w || tex_size < _h)
	{
		tex_size <<= 1;
	}

	unsigned char *td = new unsigned char[tex_size * tex_size * 4];
	for (int i = 0; i < _h; i++)
	{
		for (int j = 0; j < _w * 4; j += 4)
		{
			int yoff = i * tex_size;
			int xoff = j;

			td[yoff + j] = row_pointers[i][j];
			td[yoff + j + 1] = row_pointers[i][j + 1];
			td[yoff + j + 2] = row_pointers[i][j + 2];
			td[yoff + j + 3] = row_pointers[i][j + 3];
		}
	}
	 */
	unsigned char *td = new unsigned char[_h * _w * bpp];
	for (int i = 0; i < _w * _h * bpp; i += bpp)
	{
		int yoff = i / (_w * bpp);
		int xoff = (i - (_w * bpp) * yoff);
		for(int j = 0; j<bpp; j++)
			td[i+j] = row_pointers[yoff][xoff+j];

	}

	if(bpp == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _w, _h, 0, GL_RGBA,	GL_UNSIGNED_BYTE, td);
	else if(bpp == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _w, _h, 0, GL_RGB,	GL_UNSIGNED_BYTE, td);

	// Close the handle on the PNG file
	fclose(fin);
	glError();
	// Free the PNG image data
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete td;
	_cached[filename] = this;
}

GLImage::~GLImage()
{
	HG::Log::d("GLImage dtor");
	glDeleteTextures(1, &_texID);
}

unsigned int GLImage::w()
{
	return _w;
}

void GLImage::width(unsigned int w)
{
	_w = w;
}

unsigned int GLImage::h()
{
	return _h;
}

void GLImage::h(unsigned int h)
{
	_h = h;
}

unsigned int GLImage::get_id()
{
	return _texID;
}

float GLImage::alpha()
{
	return _a;
}

void GLImage::alpha(float a)
{
	_a = a;
}

void GLImage::tint(int tint)
{
	_tint = tint&0x00ffffff;
}

int GLImage::tint()
{
	return _tint;
}

// Declare the cache of images
map<string, GLImage*> GLImage::_cached;

}
