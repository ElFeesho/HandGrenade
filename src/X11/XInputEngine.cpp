/*
 * XInputManager.cpp
 *
 *  Created on: Jul 6, 2011
 *      Author: feesh
 */

#include "XInputEngine.h"

#include "../Engine.h"
#include "../GraphicsEngine.h"

#include "../Log.h"

namespace HandGrenade
{

XInputEngine::XInputEngine(void *window, void *display) :
	display((Display*)display), _window((Window)window), _x(0), _y(0), _buttons(0)
{

}

XInputEngine::~XInputEngine()
{
	XCloseDisplay(display);
}

unsigned int XInputEngine::mouse_x()
{
	return _x;
}

unsigned int XInputEngine::mouse_y()
{
	return _y;
}

unsigned int XInputEngine::mouse_button()
{
	int ret = 0;
	if ((_buttons & Button1Mask) == Button1Mask)
		ret |= MOUSE_LBUTTON;
	if ((_buttons & Button2Mask) == Button2Mask)
		ret |= MOUSE_MBUTTON;
	if ((_buttons & Button3Mask) == Button3Mask)
		ret |= MOUSE_RBUTTON;
	return ret;
}

bool XInputEngine::update()
{
	int unused;
	Window child_return;
	XQueryPointer(display, _window, &child_return, &child_return, &unused, &unused, &_x, &_y, &_buttons);
	return true;
}

}
