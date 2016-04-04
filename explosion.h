#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "graphics.h"
#include "model.h"
#include "vector.h"

extern Graphics * g_graphics;


class Explosion{
public:
	Explosion(void);
	~Explosion(void);

	bool Initialize(Vector position);
	void Shutdown();

	bool Render(float t);
	bool Update(float t);  //returns false if the explosion should be removed

	

private:
	Model * m_Model;

	Vector position;  

	float timer;
};

#endif