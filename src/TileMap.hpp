/*
 * TileMap.h
 *
 *  Created on: Jul 6, 2011
 *      Author: feesh
 */

#ifndef TILEMAP_H_
#define TILEMAP_H_

#include <string>
#include "Image.hpp"

using std::string;

namespace HG
{

class TileMap
{
public:
	TileMap(const string&);
	virtual ~TileMap();

	void draw(int offsetx=0, int offsety=0);
private:
	Image *_tileset;
	int _tw;
	int _th;
	int _w;
	int _h;
	int _layers;
	int _tsetw;
	unsigned char **ldata;
};

}

#endif /* TILEMAP_H_ */
