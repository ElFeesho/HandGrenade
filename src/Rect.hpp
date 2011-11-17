/*
 * Rect.h
 *
 *  Created on: Jul 3, 2011
 *      Author: feesh
 */

#ifndef RECT_H_
#define RECT_H_

namespace HG
{

struct Rect
{
public:
	Rect(int x, int y, int w, int h);
	virtual ~Rect();
	int x;
	int y;
	int w;
	int h;
};

}

#endif /* RECT_H_ */
