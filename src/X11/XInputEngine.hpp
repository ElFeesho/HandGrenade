/*
 * XInputEngine.h
 *
 *  Created on: Jul 6, 2011
 *      Author: feesh
 */

#ifndef XINPUTENGINE_H_
#define XINPUTENGINE_H_

#include <X11/Xlib.h>

#include "../InputEngine.h"

namespace HandGrenade
{

class XInputEngine: public HandGrenade::InputEngine
{
public:
	XInputEngine(void *window, void *display);
	virtual ~XInputEngine();

	unsigned int mouse_x();
	unsigned int mouse_y();

	unsigned int mouse_button();

	bool update();
private:
	Display *display;
	Window _window;
	int _x;
	int _y;
	unsigned int _buttons;
};

}

#endif /* XINPUTENGINE_H_ */
