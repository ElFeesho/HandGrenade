/*
 * LUAManager.cpp
 *
 *  Created on: Jul 3, 2011
 *      Author: feesh
 */

#include "LUAManager.hpp"
#include "../Log.hpp"
#include "LUAEntity.hpp"
#include "../Engine.hpp"
#include "../GraphicsEngine.hpp"
#include "../Sprite.hpp"
#include "../Image.hpp"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <memory.h>
}

/*
 * Script Closures
 *
 Log::d("X Value: ", x());
 */

static int log_e(struct lua_State *l)
{
	if (lua_isstring(l, -1))
	{
		HG::Log::e(lua_tostring(l, -1));
	}
	return 0;
}

static int log_i(struct lua_State *l)
{
	if (lua_isstring(l, -1))
	{
		HG::Log::i(lua_tostring(l, -1));
	}
	return 0;
}

static int log_d(struct lua_State *l)
{
	if (lua_isstring(l, -1))
	{
		HG::Log::d(lua_tostring(l, -1));
	}
	return 0;
}

static int log_w(struct lua_State *l)
{
	if (lua_isstring(l, -1))
	{
		HG::Log::w(lua_tostring(l, -1));
	}
	return 0;
}

/*
 * Sprite Lib
 */

static int sprite_update(lua_State *l)
{
	if (lua_istable(l, 1))
	{
		lua_getfield(l, 1, "_sprite_private");
		HG::Sprite *lua_sprite =
				reinterpret_cast<HG::Sprite*> (lua_touserdata(l, 2));
		lua_sprite->update();
	}
	else
	{
		HG::Log::e("UPDATING SPRITE FAILED");
	}
	return 0;
}

static int sprite_draw(lua_State *l)
{
	if (lua_isnumber(l, 2) && lua_isnumber(l, 3))
	{
		if (lua_istable(l, 1))
		{
			lua_getfield(l, 1, "_sprite_private");
			lua_getfield(l, 1, "a");
			HG::Sprite
					*lua_sprite =
							reinterpret_cast<HG::Sprite*> (lua_touserdata(
									l, 4));
			lua_sprite->alpha(lua_tonumber(l, 5));
			HG::Engine::get_instance()->get_gfx_engine()->draw_sprite(
					lua_sprite, lua_tonumber(l, 2), lua_tonumber(l, 3));
		}
	}
	else
	{
		HG::Log::e("DRAWING SPRITE FAILED");
	}
	return 0;
}

static int sprite_gc(struct lua_State *l)
{
	if (lua_isuserdata(l, 1))
	{
		HG::Sprite **sprite =
				reinterpret_cast<HG::Sprite**> (lua_touserdata(l, 1));
		delete *sprite;
		HG::Log::d("Sprite GC'd from Lua");
	}
	else
	{
		HG::Log::e("Attempted to GC a sprite which was invalid");
	}

	return 0;
}

static int new_sprite(struct lua_State *l)
{
	if (lua_isstring(l, -1))
	{
		HG::Sprite *nsprite = new HG::Sprite(
				lua_tostring(l, -1));
		lua_newtable(l);
		lua_pushstring(l, "_sprite_private");
		lua_pushlightuserdata(l, nsprite);
		lua_settable(l, -3);
		lua_pushstring(l, "draw");
		lua_pushcfunction(l, sprite_draw);
		lua_settable(l, -3);
		lua_pushstring(l, "w");
		lua_pushinteger(l, nsprite->get_section().w);
		lua_settable(l, -3);
		lua_pushstring(l, "h");
		lua_pushinteger(l, nsprite->get_section().h);
		lua_settable(l, -3);
		lua_pushstring(l, "alpha");
		lua_pushnumber(l, 1.0);
		lua_settable(l, -3);
		lua_pushstring(l, "update");
		lua_pushcfunction(l, sprite_update);
		lua_settable(l, -3);

		void *handle = lua_newuserdata(l, sizeof(void*));
		/*
		 This handle is... not so nice, but it does what it needs to do
		 Lua doesn't do garbage collection on light userdata, so we need
		 some full userdata to use as a beacon for when it is time to
		 free some surface data!
		 */
		long surf_ptr = (long) nsprite;
		memcpy(handle, (void*) &surf_ptr, sizeof(void*));
		/* Store the surface's address in the handle */
		lua_setfield(l, -2, "beacon");
		lua_getfield(l, -1, "beacon");
		lua_newtable(l);
		/* Create a blank table to store metatable functions */lua_pushcfunction(l, sprite_gc);
		lua_setfield(l, -2, "__gc");
		lua_setmetatable(l, -2);
		lua_remove(l, -1);
		return 1;
	}
	return 0;
}

/*
 * Image Lib
 */

static int image_draw(lua_State *l)
{
	int args = lua_gettop(l);
	if (lua_isnumber(l, 2) && lua_isnumber(l, 3))
	{
		if (lua_istable(l, 1))
		{
			lua_getfield(l, 1, "_image_private");
			lua_getfield(l, 1, "alpha");
			lua_getfield(l, 1, "tint");
			lua_getfield(l, 1, "r");
			HG::Image *lua_image =
					reinterpret_cast<HG::Image*> (lua_touserdata(l,
							args + 1));
			lua_image->alpha(lua_tonumber(l, args+2));
			lua_image->tint(lua_tonumber(l, args+3));
			float r = lua_tonumber(l, args+4);
			if (args <= 3)
				HG::Engine::get_instance()->get_gfx_engine()->draw_image(
						lua_image, lua_tonumber(l, 2), lua_tonumber(l, 3), lua_image->w(), lua_image->h(), r);
			else
				HG::Engine::get_instance()->get_gfx_engine()->draw_image(
						lua_image, lua_tonumber(l, 2), lua_tonumber(l, 3),
						lua_tonumber(l, 4), lua_tonumber(l, 5), r);
		}
	}
	else
	{
		HG::Log::e("DRAWING IMAGE FAILED");
	}
	return 0;
}

static int image_gc(struct lua_State *l)
{
	if (lua_isuserdata(l, 1))
	{
		HG::Image **image =
				reinterpret_cast<HG::Image**> (lua_touserdata(l, 1));
		HG::Engine::get_instance()->get_gfx_engine()->destroy_image(
				*image);
		HG::Log::d("Image GC'd from Lua");
	}
	else
	{
		HG::Log::e("Attempted to GC an image which was invalid");
	}

	return 0;
}

static int new_image(struct lua_State *l)
{
	if (lua_isstring(l, -1))
	{
		HG::Image
				*nimage =
						HG::Engine::get_instance()->get_gfx_engine()->open_image(
								lua_tostring(l, -1));
		lua_newtable(l);
		lua_pushstring(l, "_image_private");
		lua_pushlightuserdata(l, nimage);
		lua_settable(l, -3);
		lua_pushstring(l, "w");
		lua_pushinteger(l, nimage->w());
		lua_settable(l, -3);
		lua_pushstring(l, "h");
		lua_pushinteger(l, nimage->h());
		lua_settable(l, -3);
		lua_pushstring(l, "tint");
		lua_pushinteger(l, 0x00ffffff);
		lua_settable(l, -3);
		lua_pushstring(l, "r");
		lua_pushinteger(l, 0);
		lua_settable(l, -3);
		lua_pushstring(l, "alpha");
		lua_pushnumber(l, 1.0);
		lua_settable(l, -3);
		lua_pushstring(l, "draw");
		lua_pushcfunction(l, image_draw);
		lua_settable(l, -3);

		void *handle = lua_newuserdata(l, sizeof(void*));
		/*
		 This handle is... not so nice, but it does what it needs to do
		 Lua doesn't do garbage collection on light userdata, so we need
		 some full userdata to use as a beacon for when it is time to
		 free some surface data!
		 */
		long surf_ptr = (long) nimage;
		memcpy(handle, (void*) &surf_ptr, sizeof(void*));
		/* Store the surface's address in the handle */
		lua_setfield(l, -2, "beacon");
		lua_getfield(l, -1, "beacon");
		lua_newtable(l);
		/* Create a blank table to store metatable functions */lua_pushcfunction(l, image_gc);
		lua_setfield(l, -2, "__gc");
		lua_setmetatable(l, -2);
		lua_remove(l, -1);
		return 1;
	}
	return 0;
}

/*
 * EntityManager lib
 */

static int find_entity(struct lua_State *l)
{
	if (lua_isstring(l, -1))
	{
		HG::LUAEntity
				*found_entity =
						reinterpret_cast<HG::LUAEntity*> (reinterpret_cast<HG::LUAManager*> (HG::Engine::get_instance()->get_script_engine())->lookup_entity(
								lua_tostring(l, -1)));
		if (found_entity != NULL)
		{
			lua_rawgeti(l, LUA_REGISTRYINDEX, found_entity->getRef());
			return 1;
		}
	}
	return 0;
}

static int load_entity(struct lua_State *l)
{
	// This function loads a new LUAEntity, and returns the table
	// relating to that entity to the caller.
	int args = lua_gettop(l);
	if (lua_isstring(l, -1))
	{
		if(args==1)
		{
			HG::LUAEntity *nentity = reinterpret_cast<HG::LUAEntity*>(HG::Engine::get_instance()->get_script_engine()->load_entity(lua_tostring(l, -1)));
			HG::Engine::get_instance()->get_scene()->add_entity(nentity);
			lua_rawgeti(l,LUA_REGISTRYINDEX,nentity->getRef());
			return 1;
		}
		else if(args == 2 && lua_isstring(l, -2))
		{
			HG::LUAEntity *nentity = reinterpret_cast<HG::LUAEntity*>(HG::Engine::get_instance()->get_script_engine()->load_entity(lua_tostring(l, -1), lua_tostring(l, -2)));
			HG::Engine::get_instance()->get_scene()->add_entity(nentity);
			lua_rawgeti(l,LUA_REGISTRYINDEX,nentity->getRef());
			return 1;
		}
	}
	else
	{
		HG::Log::e("load was passed incorrect parameters. Expects string");
	}
	return 0;
}

namespace HG
{

LUAManager::LUAManager()
{
	// TODO Auto-generated constructor stub

}

LUAManager::~LUAManager()
{
	// TODO Auto-generated destructor stub
}

bool LUAManager::init()
{
	// Initialise the LUA VM
	l = lua_open();

	// Allow access to the standard libraries
	luaL_openlibs(l);

	// Load HG libs now
	/*
	 * HG
	 *  |
	 *   --Log
	 *   --Sprite
	 *   --Image
	 *   --<Level>
	 *   --<Network>
	 *   --<Database>
	 *
	 */lua_newtable(l);
	lua_setglobal(l, "HG");
	lua_getglobal(l, "HG");
	// Logging library
	lua_newtable(l);
	lua_setfield(l, -2, "Log");

	// Sprite Library
	lua_newtable(l);
	lua_setfield(l, -2, "Sprite");

	// Image Library
	lua_newtable(l);
	lua_setfield(l, -2, "Image");

	lua_newtable(l);
	lua_setfield(l, -2, "EntityManager");

	// Sprite implementation
	lua_getglobal(l, "HG");
	lua_getfield(l, -1, "Sprite");
	lua_pushstring(l, "new");
	lua_pushcfunction(l, new_sprite);
	lua_settable(l, -3);

	// Image implementation
	lua_getglobal(l, "HG");
	lua_getfield(l, -1, "Image");
	lua_pushstring(l, "new");
	lua_pushcfunction(l, new_image);
	lua_settable(l, -3);

	// EntityManager implementation
	lua_getglobal(l, "HG");
	lua_getfield(l, -1, "EntityManager");
	lua_pushstring(l, "find");
	lua_pushcfunction(l, find_entity);
	lua_settable(l, -3);
	lua_pushstring(l, "load");
	lua_pushcfunction(l, ::load_entity);
	lua_settable(l, -3);

	return true;
}

Entity *LUAManager::load_entity(const string &filename, const string &name)
{
	LUAEntity *entity = new LUAEntity(filename, name);
	if (name != "")
		named_entities[name] = entity;
	return entity;
}

Entity *LUAManager::lookup_entity(const string &name)
{
	return named_entities[name];
}

struct lua_State *LUAManager::get_lua_vm()
{
	return l;
}

}
