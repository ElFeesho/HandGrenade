/*
 * LUAManager.h
 *
 *  Created on: Jul 3, 2011
 *      Author: feesh
 */

#ifndef LUAMANAGER_H_
#define LUAMANAGER_H_

#include "../Entity.hpp"
#include "../ScriptEngine.hpp"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <string>
#include <map>

using std::map;
using std::string;

namespace HG
{

class LUAManager : public ScriptEngine
{
public:
	LUAManager();
	virtual ~LUAManager();

	bool init();
	Entity *load_entity(const string &filename, const string &name = "");

	Entity *lookup_entity(const string &name);

	struct lua_State *get_lua_vm();
private:
	struct lua_State *l;

	map<string, Entity*> named_entities;
};

}

#endif /* LUAMANAGER_H_ */
