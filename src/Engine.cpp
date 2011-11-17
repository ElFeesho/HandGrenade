/*
 * Engine.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#include "Engine.hpp"
#include "Log.hpp"

#include <vector>
#include <string>
#include <sstream>

using std::string;
using std::vector;
using std::stringstream;


#include "Entity.hpp"

namespace HG
{

Engine::Engine() : _gfx_engine(NULL), _script_engine(NULL), _scene(NULL), _last_tick(0)
{
	Log::i("Engine ctor");
	ftime(&_time);
	_start_ticks = _time.time*1000+_time.millitm;
}

Engine::~Engine()
{
	Log::i("Engine dtor");
}

// Static methods for instance acquisition and creation
bool Engine::init()
{
	if (Engine::instance == NULL)
		Engine::instance = new Engine();

	return Engine::instance != NULL;
}

bool Engine::shutdown()
{
	if (Engine::instance != NULL)
	{
		delete Engine::instance;
		Engine::instance = NULL;
	}

	return Engine::instance == NULL;
}

Engine* Engine::get_instance()
{
	return Engine::instance;
}

// Instance methods
void Engine::set_gfx_engine(GraphicsEngine *gfx_engine)
{
	_gfx_engine = gfx_engine;
}

GraphicsEngine *Engine::get_gfx_engine()
{
	return _gfx_engine;
}

void Engine::set_script_engine(ScriptEngine *script_engine)
{
	_script_engine = script_engine;
}

ScriptEngine *Engine::get_script_engine()
{
	return _script_engine;
}

bool Engine::update()
{
	static long tick = 0;
	tick = get_ticks();
	if(_scene == NULL)
	{
		Log::w("Began engine update without a scene");
		return false;
	}

	if(_scene->update())
	{
		// 60fps = 1000/60 = 16.6
		long to_sleep = 16-(get_ticks()-tick);
		if(to_sleep<=0)
			to_sleep = 0;
		usleep(to_sleep*1000);
		return true;
	}
	else
	{
		delete _scene;
	}

	return false;
}

void Engine::set_scene(Scene *scene)
{
	Log::d("Set the scene");
	_scene = scene;
}

Scene *Engine::get_scene()
{
	return _scene;
}

long Engine::get_ticks()
{
	ftime(&_time);
	return (_time.time*1000+_time.millitm)-_start_ticks;
}

// Static member initialisers
Engine *Engine::instance = NULL;

}
