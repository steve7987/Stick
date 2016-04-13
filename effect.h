#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "vector.h"

extern Graphics * g_graphics;


class Effect{
public:
	
	virtual void Shutdown() = 0;

	virtual bool Render(float t) = 0;
	virtual bool Update(float t) = 0;  //returns false if the effect should be removed
};

#endif