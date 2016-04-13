#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "graphics.h"
#include "model.h"
#include "vector.h"
#include "effect.h"

extern Graphics * g_graphics;


class Explosion : public Effect{
public:
	Explosion(void);
	~Explosion(void);

	bool Initialize(Vector position, Vector scale, float duration);
	void Shutdown();

	bool Render(float t);
	bool Update(float t);  //returns false if the explosion should be removed

	

private:
	Model * m_Model;

	Vector position;  
	Vector scale;

	float timer;  //time since explosion was created

	float duration;  //how long the explosion should take
};

#endif