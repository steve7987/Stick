#ifndef _HERO_H_
#define _HERO_H_

#include "graphics.h"
#include "input.h"
#include "model.h"
#include "vector.h"
#include "helpers.h"

extern Graphics * g_graphics;

class Hero
{
public:
	Hero(void);
	~Hero(void);

	bool Initialize(Vector position, Vector softBoundary);
	void Shutdown();

	bool Render(float t);
	//velocity should come from the keyboard
	//blockDeque is a deque of blocks that can be collided with
	void Update(float t, Input * input);   

	Vector GetPosition();
	Vector GetVelocity();

private:
	

	//model for the hero
	Model * heroModel;
	Model *sbModel;  //transparent cube to represent the soft boundary for the ship
	
	Vector position;
	Vector velocity;  //velocity is in units per second

	Vector softBoundary;  //outside this boundry (box from -sb to sb) ship wont accelerate
};

#endif