/*
 * LUAEntity.cpp
 *
 *  Created on: Jul 3, 2011
 *      Author: feesh
 */

#include "LUAEntity.hpp"

#include "../Engine.hpp"
#include "LUAManager.hpp"
#include "../Log.hpp"

namespace HG
{

LUAEntity::LUAEntity()
{
	l = reinterpret_cast<LUAManager*> (Engine::get_instance()->get_script_engine())->get_lua_vm();
}

LUAEntity::LUAEntity(const string &filename, const string &name) :
	_x(0), _y(0)
{
	// Store the name of the entity so it may be looked up later
	this->name = name;

	// Initialise the LUA VM
	l = reinterpret_cast<LUAManager*> (Engine::get_instance()->get_script_engine())->get_lua_vm();

	// Allow access to the standard libraries
	int err = luaL_loadfile(l, filename.c_str());
	if (err)
	{
		Log::e("Error loading: " + filename, lua_tostring(l, -1));
	}
	else
	{
		Log::i("Loaded " + filename + " successfully");
	}

	err = lua_pcall(l, 0, 0, 0);
	if (err)
	{
		Log::e("Error parsing: " + filename, lua_tostring(l, -1));
	}
	else
	{
		Log::i("Parsed " + filename + " successfully");
	}

	lua_getglobal(l, "init");
	err = lua_pcall(l, 0, 1, 0);
	if (err)
	{
		Log::e("Error running init function: ", lua_tostring(l, -1));
		return;
	}
	else
	{
		Log::d("Initialised LUAEntity: "+filename);
	}
	// On the top of the stack now should be a table for the entity
	if (!lua_istable(l, -1))
	{
		Log::e("Init function didn't return a table!");
	}

	_ref = luaL_ref(l, LUA_REGISTRYINDEX);
	/*
	 lua_rawgeti(l, LUA_REGISTRYINDEX, _ref);
	 // Imbue the new entity with x and y functions
	 lua_pushstring(l, "x");
	 lua_pushcfunction(l,_LUAx);
	 lua_settable(l, -3);
	 lua_pushstring(l, "y");
	 lua_pushcfunction(l,_LUAy);
	 lua_settable(l, -3);

	 Log::d("Duck punched entity methods into entity table");*/

	// Get the table, set the X and Y of the entity
	lua_rawgeti(l, LUA_REGISTRYINDEX, _ref);
	lua_pushstring(l, "x");
	lua_pushinteger(l, _x);
	lua_settable(l, -3);
	lua_pushstring(l, "y");
	lua_pushinteger(l, _y);
	lua_settable(l, -3);
}

LUAEntity::~LUAEntity()
{
}

bool LUAEntity::update()
{
	// Update the _current pointer to point to this object
	// otherwise C functions called from the source will not
	// execute on this object

	// Get the table for the entity and pop it on the stack
	lua_rawgeti(l, LUA_REGISTRYINDEX, _ref);

	lua_getfield(l, -1, "update");

	// get the table again ... it's passed in as the first object (the self parameter)
	lua_rawgeti(l, LUA_REGISTRYINDEX, _ref);
	lua_pushinteger(l, Engine::get_instance()->get_ticks());
	int err = lua_pcall(l, 2, 1, 0);
	if (err)
	{
		Log::e("Error parsing update function: ", lua_tostring(l, -1));
		return false;
	}

	if(lua_isboolean(l, -1))
	{
		if(lua_toboolean(l, -1))
		{
			// Clear the stack ... todo fix this problem!
			lua_settop(l, 0);
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

int LUAEntity::getRef()
{
	return _ref;
}

void LUAEntity::x(int x)
{
	_x = x;

	// Update LUA object
	lua_rawgeti(l, LUA_REGISTRYINDEX, _ref);
	lua_pushinteger(l, _x);
	lua_setfield(l, -2, "x");
}

int LUAEntity::x()
{
	lua_rawgeti(l, LUA_REGISTRYINDEX, _ref);
	lua_getfield(l, -1, "x");
	if (!lua_isnumber(l, -1))
	{
		Log::e("X is not an integer, resetting to 0");
		x(0);
	}
	else
	{
		_x = lua_tointeger(l, -1);
	}

	return _x;
}

void LUAEntity::y(int y)
{
	_y = y;
}

int LUAEntity::y()
{
	return _y;
}

}
