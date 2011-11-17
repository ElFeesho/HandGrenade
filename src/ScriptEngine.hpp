/*
 * ScriptEngine.h
 *
 *  Created on: Jul 4, 2011
 *      Author: feesh
 */

#ifndef SCRIPTENGINE_H_
#define SCRIPTENGINE_H_

#include "Entity.hpp"
#include <string>

using std::string;

namespace HG
{

class ScriptEngine
{
public:
	ScriptEngine();
	virtual ~ScriptEngine();

	virtual bool init() = 0;

	virtual Entity *load_entity(const string &filename, const string &name = "") = 0;
};

}

#endif /* SCRIPTENGINE_H_ */
