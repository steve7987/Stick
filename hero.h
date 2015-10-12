#ifndef _HERO_H_
#define _HERO_H_

#include <deque>

#include "graphics.h"
#include "input.h"
#include "model.h"
#include "vector.h"
#include "helpers.h"
#include "block.h"

extern Graphics * g_graphics;


class Hero
{
public:
	Hero(void);
	~Hero(void);

	bool Initialize(Vector position);
	void Shutdown();

	bool Render(float t);
	//velocity should come from the keyboard
	//blockDeque is a deque of blocks that can be collided with
	void Update(float t, Input * input, deque<Block *> * blockDeque );   

	Vector GetPosition();
	Vector GetAnchorVector();
	Block * GetAnchorBlock();

private:
	//checks for collisions with any block in the list, if a collision occurs, position and velocity are adjusted
	void checkCollisions(deque<Block *> * blockDeque);
	//knowing that there has been a collision with b, resolve the collision by changing position and velocity
	void resolveCollision(Block * b);

	Model * heroModel;

	Vector position;
	Vector velocity;  //velocity is in units per second
	float radius;

	//the block that the hero is attached to, 0 if in the air
	Block * anchorBlock;
	//contains the faces of the block that the hero is anchored to (e.g. <0,1,-1> means far y and near z planes)
	Vector anchorVector; 
};

#endif