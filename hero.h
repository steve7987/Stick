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

	bool Initialize(Vector position);
	void Shutdown();

	bool Render(float t);
	//velocity should come from the keyboard
	//blockDeque is a deque of blocks that can be collided with
	void Update(float t, Input * input);   

	Vector GetPosition();
	

private:
	

	//model for the hero
	Model * heroModel;
	
	Vector position;
	Vector velocity;  //velocity is in units per second
};

#endif