/*
 * Scene.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#include "Scene.hpp"
#include "Log.hpp"

namespace HG
{

Scene::Scene()
{

}

Scene::~Scene() 
{

}

bool Scene::update()
{
	for (vector<Entity *>::iterator it = _entity_list.begin(); it
			!= _entity_list.end(); ++it)
	{
		if (!(*it)->update())
		{
			Log::i("Deleting dead entity");
			// Free up the entity
			delete (*it);

			// Remove the pointer from the vector and update the iterator's position
			it = _entity_list.erase(it);
			Log::i("Entity Count: ", _entity_list.size());

			if (it == _entity_list.end())
				break;
		}
	}
	for(vector<Entity *>::iterator it = _pending_list.begin(); it!=_pending_list.end(); )
	{
		_entity_list.push_back((*it));
		it = _pending_list.erase(it);
	}
	return _entity_list.size()>0;
}

void Scene::add_entity(Entity *entity)
{
	_pending_list.push_back(entity);
}

}
