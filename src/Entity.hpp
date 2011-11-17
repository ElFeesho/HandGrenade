/*
 * Entity.h
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#ifndef ENTITY_H_
#define ENTITY_H_

namespace HG
{

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual bool update() = 0;

	virtual int x();
	virtual void x(int);

	virtual int y();
	virtual void y(int);

protected:
	int _x;
	int _y;
};

}

#endif /* ENTITY_H_ */
