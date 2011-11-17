/*
 * Entity.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#include "Entity.hpp"

namespace HG
{

Entity::Entity()
{

}

Entity::~Entity()
{

}

int Entity::x()
{
	return _x;
}

void Entity::x(int _x)
{
	this->_x = _x;
}

int Entity::y()
{
	return _y;
}

void Entity::y(int _y)
{
	this->_y = _y;
}

}
