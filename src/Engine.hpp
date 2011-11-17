/*
 * Engine.h
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include "Scene.hpp"

#include <sys/timeb.h>

#include <vector>

using std::vector;

namespace HG
{

class GraphicsEngine;
class ScriptEngine;

class Engine
{
public:
	static bool init();
	static bool shutdown();

	static Engine *get_instance();

	// Instance methods
	// Graphics engine methods
	void 			set_gfx_engine(GraphicsEngine *gfx_engine);
	GraphicsEngine *get_gfx_engine();

	// Script engine methods
	void set_script_engine(ScriptEngine *script_engine);
	ScriptEngine *get_script_engine();

	bool update();

	// Entity list modifying methods
	void set_scene(Scene *);
	Scene *get_scene();

	// Timekeeping
	long get_ticks();
private:
	// Prevent multiple instances being made of Engine
	Engine();
	~Engine();

	GraphicsEngine *_gfx_engine;
	ScriptEngine *_script_engine;

	Scene *_scene;

	// Time keeping members
	struct timeb _time;
	long _start_ticks;
	long _last_tick;

	// Static members
	static Engine *instance;
};

}

#endif /* ENGINE_H_ */
