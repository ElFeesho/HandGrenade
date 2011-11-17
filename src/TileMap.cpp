/*
 * TileMap.cpp
 *
 *  Created on: Jul 6, 2011
 *      Author: feesh
 */

#include "TileMap.hpp"
#include "Engine.hpp"
#include "GraphicsEngine.hpp"
#include "Log.hpp"
#include <fstream>

using std::ifstream;

namespace HG
{

TileMap::TileMap(const string &level_file)
{
	ifstream level(level_file.c_str());

	if(level.good())
	{
		Log::i("Opened "+level_file+" successfully");
		string tileset_img;
		level >> tileset_img;
		level >> _tw;
		level >> _th;
		level >> _w;
		level >> _h;

		Log::i("Tileset Img: "+tileset_img);
		Log::i("Tileset tile width: ", _tw);
		Log::i("Tileset tile height: ", _th);
		Log::i("Level width: ", _w);
		Log::i("Level height: ", _h);

		_tileset = Image::open_image(tileset_img);
		_tsetw = _tileset->w()/_tw;


		level.ignore(1); // Skip the last new line character
		ldata = new unsigned char*[_h];
	
		for(int i = 0; i<_h; i++)
		{
			ldata[i] = new unsigned char[_w];
			level.read((char*)ldata[i],_w);
			level.ignore(1);
		}

		level.close();
	}
	else
	{
		Log::e("Couldn't open: "+level_file);
	}
}

TileMap::~TileMap()
{
	delete _tileset;
	for(int i = 0;i<_h; i++)
		delete[] ldata[i];
	delete[] ldata;

}

void TileMap::draw(int offsetx, int offsety)
{
	int x = 0;
	int y = 0;

	for(int i = 0;i<_h; i++)
	{
		y = offsety + i * _th;
		for(int j = 0;j<_w; j++)
		{
			if(ldata[i][j]<'a')
				continue;
			x = offsetx + j*_tw;
			int pos = (ldata[i][j]-'a');
			int sx = (pos%_tsetw)*_tw;
			int sy = (pos/_tsetw)*_th;
			_tileset->draw_subsection(x, y, _tw, _th, sx, sy, _tw, _th);
		}
	}
}

}
