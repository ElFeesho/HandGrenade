/*
 * Scene.h
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#ifndef SCENE_H_
#define SCENE_H_

#include "Entity.hpp"

#include <vector>

using std::vector;

namespace HG
{

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual bool update();

	void add_entity(Entity *);
private:
	vector<Entity *> _entity_list;
	vector<Entity *> _pending_list;
};

}

#endif /* SCENE_H_ */
