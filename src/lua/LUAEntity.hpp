/*
 * LUAEntity.h
 *
 *  Created on: Jul 3, 2011
 *      Author: feesh
 */

#ifndef LUAENTITY_H_
#define LUAENTITY_H_

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "../Entity.hpp"

#include <string>

using std::string;



namespace HG
{

class LUAEntity: public HG::Entity
{
public:
	LUAEntity();
	LUAEntity(const string &, const string &);
	virtual ~LUAEntity();

	bool update();

	void x(int x);
	int x();

	void y(int y);
	int y();

	// Get the LUA Reference to the entity's table
	int getRef();

	// LUA Closures
	int LUAx(lua_State *l);
	int LUAy(lua_State *l);

private:
	struct lua_State *l;

	string name;

	int _x;
	int _y;

	// The reference to the LUA table that represents the entity
	// returned from the script's init function
	int _ref;

};

}

#endif /* LUAENTITY_H_ */
