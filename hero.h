#ifndef _HERO_H_
#define _HERO_H_

#include "graphics.h"
#include "model.h"
#include "vector.h"

extern Graphics * g_graphics;


class Hero
{
public:
	Hero(void);
	~Hero(void);

	bool Initialize(Vector position);
	void Shutdown();

	bool Render(float t);
	void Update(float t, Vector velocity);  //velocity should come from the keyboard 

	Vector GetPosition();

private:
	Model * heroModel;

	Vector position;

};

#endif